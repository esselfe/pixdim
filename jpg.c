#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <magic.h>
#include <jpeglib.h>

#include "pixdim.h"

void pixdimJPG_ShowSize (char *filename) {
	unsigned int width, height, pixel_size;
	if (pixdimJPG_GetSize (filename, &width, &height, &pixel_size))
		return;
	if (width == 0 || height == 0 || pixel_size == 0) {
		width = 0;
		height = 0;
		pixel_size = 0;
	}
	if (pixdim_options & PIXDIM_OPTION_DEBUG)
		printf ("%d %d %d size:%lu inode:%lu %s\n", width, height, pixel_size,
			pixdimStatFilesize(filename), pixdimStatInode(filename), filename);
	else if (pixdim_options & PIXDIM_OPTION_VERBOSE)
		printf ("%d %d %d %s\n", width, height, pixel_size, filename);
	else
		printf ("%d %d\n", width, height);
}

int pixdimJPG_GetSize (char *filename, unsigned int *width, 
	unsigned int *height, unsigned int *pixel_size) {
	struct stat jpg_stat;
	int jpg_fd, row_stride, bmp_size;
	unsigned long jpg_size;
	unsigned char *jpg_buffer, *bmp_buffer;
	struct jpeg_decompress_struct jinfo;
	struct jpeg_error_mgr jerr;

	// Check the validity of the file extension against the magic file type
/*	magic_t mgc = magic_open (MAGIC_SYMLINK | MAGIC_PRESERVE_ATIME | MAGIC_MIME_TYPE);
	if (mgc != NULL) {
		magic_load (mgc, NULL);
		if (strcmp(magic_file (mgc, filename),"image/png")==0) {
			magic_close (mgc);
			return pixdimPNG_GetSize (filename, width, height, pixel_size);
		}
	}
*/
	*width = 0;
	*height = 0;
	if (pixel_size != NULL)
		*pixel_size = 0;

	if (stat (filename, &jpg_stat)) {
		fprintf (stderr, "%s:%s() error: Cannot open %s: %s\n",
			pixdim_name, __FUNCTION__, filename, strerror(errno));
		return errno;
	}
	jpg_size = jpg_stat.st_size;
	jpg_buffer = malloc (jpg_size + 100);

	jpg_fd = open (filename, O_RDONLY);
	off_t jpg_bytes_read = read (jpg_fd, jpg_buffer, jpg_size);
	close (jpg_fd);
	if (pixdim_options & PIXDIM_OPTION_DEBUG)
		printf("jpg_bytes_read: %lu\n", jpg_bytes_read);

	jinfo.err = jpeg_std_error (&jerr);
	jpeg_create_decompress (&jinfo);
	jpeg_mem_src (&jinfo, jpg_buffer, jpg_size);
	jpeg_read_header (&jinfo, 1);
	jpeg_start_decompress (&jinfo);

	*width = (unsigned int)jinfo.output_width;
	*height = (unsigned int)jinfo.output_height;
	if (pixel_size != NULL)
		*pixel_size = jinfo.output_components;

	// libjpeg.so complains if there are no scanline read, so make everyone happy
	row_stride = (*width) * (*pixel_size);
	bmp_size = (*width) * (*height) * jinfo.output_components;
	bmp_buffer = malloc (bmp_size);
	while (jinfo.output_scanline < jinfo.output_height) {
		unsigned char *buffer_array[1];
		buffer_array[0] = bmp_buffer + (jinfo.output_scanline) * row_stride;
		jpeg_read_scanlines (&jinfo, buffer_array, 1);
	}
	
	jpeg_finish_decompress (&jinfo);
	jpeg_destroy_decompress (&jinfo);
	free (jpg_buffer);
	free (bmp_buffer);

	return 0;
}


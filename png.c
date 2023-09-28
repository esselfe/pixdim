#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <png.h>
#include <magic.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "pixdim.h"

void pixdimPNG_ShowSize (char *filename) {
	unsigned int width, height, pixel_size;
	if (pixdimPNG_GetSize (filename, &width, &height, &pixel_size))
		return;
	/*else if (width == 0 || height == 0 || pixel_size == 0) {
		width = 0;
		height = 0;
		pixel_size = 0;
	}*/

	if (pixdim_options & PIXDIM_OPTION_DEBUG)
		printf ("%u %u %u %s\n", width, height, pixel_size, filename);
	else if (pixdim_options & PIXDIM_OPTION_VERBOSE)
		printf ("%u %u %u %s\n", width, height, pixel_size, filename);
	else
		printf ("%u %u %s\n", width, height, filename);
}

int pixdimPNG_GetSize (char *filename, unsigned int *width,
	unsigned int *height, unsigned int *pixel_size) {
	// Check the validity of the file extension against the magic file type
/*	magic_t mgc = magic_open (MAGIC_SYMLINK | MAGIC_PRESERVE_ATIME | MAGIC_MIME_TYPE);
	if (mgc != NULL) {
		magic_load (mgc, NULL);
		if (strcmp(magic_file (mgc, filename),"image/jpeg")==0) {
			magic_close (mgc);
			return pixdimJPG_GetSize (filename, width, height, pixel_size);
		}
	}
*/
	*width = 0;
	*height = 0;
	if (pixel_size != NULL)
		*pixel_size = 0;
	
	FILE *fp = fopen (filename, "rb");
	if (fp == NULL) {
		fprintf (stderr, "%s:%s() error: Cannot open %s: %s\n", pixdim_name,
			__FUNCTION__, filename, strerror(errno));
		return 1;
	}

	png_structp png = png_create_read_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) {
		fprintf (stderr, "%s:%s() error: png_create_read_struct() failed\n", 
			pixdim_name, __FUNCTION__);
		return 1;
	}

	png_infop info = png_create_info_struct (png);
	if (!info) {
		fprintf (stderr, "%s:%s() error: png_create_info_struct() failed\n",
			pixdim_name, __FUNCTION__);
		return 1;
	}

	if (setjmp(png_jmpbuf(png))) {
		fprintf (stderr, "%s:%s() error: setjmp(png_jmpbuf(png)) failed\n",
			pixdim_name, __FUNCTION__);
		return 1;
	}

	png_init_io (png, fp);
	png_read_info (png, info);
	
	*width = png_get_image_width(png, info);
	*height = png_get_image_height(png, info);
	if (pixel_size != NULL)
		*pixel_size = (unsigned int)png_get_channels(png, info);

	png_destroy_read_struct (&png, &info, NULL);
	fclose (fp);
	return 0;
}


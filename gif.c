#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <gif_lib.h>

#include "pixdim.h"

int pixdimGIF_GetSize(char *filename, unsigned int *width,
	unsigned int *height, unsigned int *pixel_size) {
	int errorcode = 0;
	GifFileType *gifptr = DGifOpenFileName(filename, &errorcode);
	if (errorcode != 0) {
		printf("pixdim error: %s\n", GifErrorString(errorcode));
		*width = 0;
		*height = 0;
		return 1;
	}
	
	*width = gifptr->SWidth;
	*height = gifptr->SHeight;
	*pixel_size = gifptr->SColorMap->BitsPerPixel;
	
	DGifCloseFile(gifptr, &errorcode);
	return 0;
}

void pixdimGIF_ShowSize(char *filename) {
	unsigned int width, height, pixel_size;

	if (pixdimGIF_GetSize(filename, &width, &height, &pixel_size))
		return;
	
	if (pixdim_options & PIXDIM_OPTION_DEBUG)
		printf ("%d %d %d size:%lu inode:%lu %s\n", width, height, pixel_size,
			pixdimStatFilesize(filename), pixdimStatInode(filename), filename);
	else if (pixdim_options & PIXDIM_OPTION_VERBOSE)
		printf ("%d %d %d %s\n", width, height, pixel_size, filename);
	else
		printf ("%d %d %s\n", width, height, filename);
	
	return;
}


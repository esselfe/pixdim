#ifndef PICSIZE_H
#define PICSIZE_H 1

#include <sys/types.h>

extern const char *pixdim_name;
extern unsigned int pixdim_options;

#define PIXDIM_OPTION_DEBUG      1
#define PIXDIM_OPTION_VERBOSE    (1<<1)

// pixdim.c
void pixdimVersionStringShow (void);

// jpg.c
int pixdimJPG_GetSize (char *filename, unsigned int *width,
	unsigned int *height, unsigned int *pixel_size);
void pixdimJPG_ShowSize (char *filename);

// png.c
int pixdimPNG_GetSize (char *filename, unsigned int *width,
	unsigned int *height, unsigned int *pixel_size);
void pixdimPNG_ShowSize (char *filename);

// stat.c
off_t pixdimStatFilesize(char *filename);
ino_t pixdimStatInode(char *filename);

#endif /* PICSIZE_H */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "pixdim.h"

static struct stat st01;

off_t pixdimStatFilesize(char *filename) {
	if (pixdim_options & PIXDIM_OPTION_DEBUG)
		printf("pixdim stat off_t type size: %lu\n", sizeof(off_t));

	if (stat(filename,&st01) < 0) {
		fprintf(stderr,"pixdimStatFilesize(): Cannot open %s: %s\n",
			filename, strerror(errno));
		return 0;
	}

	return st01.st_size;
}

/* returns filesystem inode number of file
(ino_t == unsigned long) */
ino_t pixdimStatInode(char *filename) {
	if (pixdim_options & PIXDIM_OPTION_DEBUG)
		printf("pixdim stat ino_t type size: %lu\n", sizeof(ino_t));
	
	if (stat(filename,&st01) < 0) {
		fprintf(stderr,"pixdimStatInode(): Cannot open %s: %s\n",
			filename, strerror(errno));
		return 0;
	}

	return st01.st_ino;
}


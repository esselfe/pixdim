#include <stdio.h>

#include "pixdim.h"

const char *pixdim_name = "pixdim";
static const char *pixdim_version_string = "0.1.6";
unsigned int pixdim_options;

void pixdimVersionStringShow (void) {
	printf ("%s %s\n", pixdim_name, pixdim_version_string);
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <magic.h>

#include "pixdim.h"

static const struct option long_options[] = {
	{"debug", no_argument, NULL, 'D'},
    {"help", no_argument, NULL, 'h'},
    {"verbose", no_argument, NULL, 'v'},
    {"version", no_argument, NULL, 'V'},
    {NULL, 0, NULL, 0}
};
static const char *short_options = "DhvV";

void pixdimHelpShow (void) {
    printf ("Usage: %s [ --help/-h | --debug/-D | --verbose/-v | --version/-V ]\n"
        "\tpixel_size is shown with the verbose option\n", pixdim_name);
}

int main (int argc, char **argv) {
    if (argc < 2) {
        pixdimHelpShow ();
        exit (EINVAL);
    }

    int c, argcnt = 0;
    while (1) {
        c = getopt_long (argc, argv, short_options, long_options, NULL);
        if (c == -1) break;
        switch (c) {
		case 'D':
			pixdim_options |= PIXDIM_OPTION_DEBUG | PIXDIM_OPTION_VERBOSE;
			break;
        case 'h':
            pixdimHelpShow ();
            exit (0);
            break;
        case 'v':
            ++argcnt;
            pixdim_options |= PIXDIM_OPTION_VERBOSE;
            break;
        case 'V':
            pixdimVersionStringShow ();
            exit (0);
            break;
        default:
            fprintf (stderr, "%s error: Unknown am argument: #%d/c::%c::\n", pixdim_name, c, (char)c);
            break;
        }
    }
    if ((argc-argcnt) < 2) {
        pixdimHelpShow ();
        exit (EINVAL);
    }

	magic_t mgc = magic_open (MAGIC_SYMLINK | MAGIC_PRESERVE_ATIME | MAGIC_MIME_TYPE);
	if (mgc == NULL) {
		fprintf (stderr, "%s: Cannot determine magic file type\n", pixdim_name);
		exit (ECANCELED);
	}
	magic_load (mgc, NULL);
    int cnt;
	char *magic_str;
    for (cnt = 1; cnt < argc; cnt++) {
		magic_str = (char *)magic_file (mgc, argv[cnt]);
		if (magic_str != NULL) {
			if (strcmp(magic_str,"image/jpeg")==0)
                pixdimJPG_ShowSize (argv[cnt]);
			else if (strcmp(magic_str,"image/png")==0)
                pixdimPNG_ShowSize (argv[cnt]);
		}
    }

    return 0;
}


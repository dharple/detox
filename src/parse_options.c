/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

#include "detox_struct.h"

#include "filelist.h"
#include "parse_options.h"
#include "wrapped.h"

#ifdef HAVE_GETOPT_LONG
#include <getopt.h>
#endif

enum {
    LONG_OPTION_INLINE = 1,
    LONG_OPTION_RECURSIVE,
    LONG_OPTION_SPECIAL
};

#define INLINE_DETOX_BIN "inline-detox"

/* expect this to be overwritten! */
static int long_option = 0;

#ifdef HAVE_GETOPT_LONG
static struct option longopts[] = {

    /* long options with equivalents */
    {"dry-run", no_argument, NULL, 'n'},
    {"help", no_argument, NULL, 'h'},
    {"verbose", no_argument, NULL, 'v'},

    /* long options without */
    {"inline", no_argument, &long_option, LONG_OPTION_INLINE},
    {"recursive", no_argument, &long_option, LONG_OPTION_RECURSIVE},
    {"special", no_argument, &long_option, LONG_OPTION_SPECIAL},

    /* done */
    {NULL, 0, NULL, 0}
};

#endif

/* *INDENT-OFF* */

char usage_message[] = {
     "usage: detox [-hLnrvV] [-f configfile] [-s sequence]"
#ifdef HAVE_GETOPT_LONG
     " [--dry-run] [--help] [--inline] [--recursive] [--special] [--verbose]"
     "\n\t "
#endif
     " file [file ...]\n"
};

char help_message[] = {
     "	-f configfile	choose which config file to use\n"
#ifdef HAVE_GETOPT_LONG
    "	-h --help	this message\n"
#else
    "	-h 		this message\n"
#endif
#ifdef HAVE_GETOPT_LONG
    "	--inline	run inline mode\n"
#endif
    "	-L		list available sequences and exit\n"
    "			with -v ... dump sequence contents\n"
#ifdef HAVE_GETOPT_LONG
    "	-n --dry-run	do a dry run (don't actually do anything)\n"
    "	-r --recursive	be recursive (descend into subdirectories)\n"
#else
    "	-n 		do a dry run (don't actually do anything)\n"
    "	-r 		be recursive (descend into subdirectories)\n"
#endif
    "	-s sequence	choose which sequence to detox with\n"
#ifdef HAVE_GETOPT_LONG
    "	--special	work on links and special files\n"
#endif
    "	-v --verbose 	be verbose\n"
    "	-V 		show the current version\n"
};

char usage_message_inline[] = {
    "usage: inline-detox [-hLvV] [-f configfile] [-s sequence] [file]\n"
};

char help_message_inline[] = {
    "	-f configfile	choose which config file to use\n"
#ifdef HAVE_GETOPT_LONG
    "	-h --help	this message\n"
#else
    "	-h 		this message\n"
#endif
    "	-L		list available sequences and exit\n"
    "			with -v ... dump sequence contents\n"
    "	-s sequence	choose which sequence to detox with\n"
    "	-v --verbose 	be verbose\n"
    "	-V 		show the current version\n"
};

/* *INDENT-ON* */

options_t *options_init(void)
{
    options_t *ret;

    ret = wrapped_malloc(sizeof(options_t));

    memset(ret, 0, sizeof(options_t));

    return ret;
}

options_t *parse_options_getopt(int argc, char **argv)
{
    int optcode;
    options_t *main_options;
    char *binname;

    main_options = options_init();
    main_options->sequence_name = getenv("DETOX_SEQUENCE");

    binname = basename(argv[0]);
    main_options->is_inline_bin = main_options->is_inline_mode =
                                      (strcmp(binname, INLINE_DETOX_BIN) == 0);

#ifdef HAVE_GETOPT_LONG
    while ((optcode = getopt_long(argc, argv, "hrvV?Ls:f:n", longopts, NULL)) != -1) {
#else
    while ((optcode = getopt(argc, argv, "hrvV?Ls:f:n")) != -1) {
#endif
        switch (optcode) {
            case 'h':
                printf("%s", !main_options->is_inline_bin ? usage_message :
                       usage_message_inline);
                printf("\n");
                printf("%s", !main_options->is_inline_bin ? help_message :
                       help_message_inline);
                exit(EXIT_SUCCESS);

            case 'f':
                /*
                 * XXX - free multiple check_config_files
                 */
                main_options->check_config_file = wrapped_strdup(optarg);
                break;

            case 'L':
                main_options->list_sequences = 1;
                break;

            case 'n':
                main_options->dry_run = 1;
                break;

            case 'r':
                main_options->recurse = 1;
                break;

            case 's':
                /*
                 * XXX - free multiple sequence name opts
                 */
                main_options->sequence_name = wrapped_strdup(optarg);
                break;

            case 'v':
                main_options->verbose++;
                break;

            case 'V':
                printf("%s\n", PACKAGE_STRING);
                exit(EXIT_SUCCESS);

            case '?':
                printf("%s", !main_options->is_inline_bin ? usage_message :
                       usage_message_inline);
                exit(EXIT_SUCCESS);

            case 0:
                switch (long_option) {
                    case LONG_OPTION_INLINE:
                        main_options->is_inline_mode = 1;
                        break;

                    case LONG_OPTION_RECURSIVE:
                        main_options->recurse = 1;
                        break;

                    case LONG_OPTION_SPECIAL:
                        main_options->special = 1;
                        break;

                    default:
                        /*
                         * getopt_long shouldn't let us get here...
                         * verify?
                         */
                        printf("unknown option: %s\n", optarg);
                        break;
                }
                long_option = 0; /* clean up! */
                break;

            default:
                fprintf(stderr, "unknown option: %c\n", optcode);
                exit(EXIT_FAILURE);
        }
    }

    if (main_options->list_sequences) {
        /*
         * Early Retirement
         */
        return main_options;
    }

    main_options->files = filelist_init();

    if (optind < argc) {
        while (optind < argc) {
            filelist_put(main_options->files, argv[optind]);
            optind++;
        }
    } else if (!main_options->is_inline_mode) {
        printf("%s", !main_options->is_inline_bin ? usage_message :
               usage_message_inline);
        exit(EXIT_FAILURE);
    }

    return main_options;
}

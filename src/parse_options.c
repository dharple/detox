/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "config.h"

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <libgen.h>

#include "detox.h"
#include "config.h"
#include "parse_options.h"

#ifdef HAVE_GETOPT_LONG
#include <getopt.h>
#endif

enum {
	LONG_OPTION_DRY_RUN = 1,
	LONG_OPTION_INLINE,
	LONG_OPTION_REMOVE_TRAILING,
	LONG_OPTION_SPECIAL
};

#define INLINE_DETOX_BIN "inline-detox"

/* expect this to be overwritten! */
static int long_option = 0;

#ifdef HAVE_GETOPT_LONG
static struct option longopts[] = {

	/* long options with equivalents */
	{"help", no_argument, 0, 'h'},
	{"dry-run", no_argument, 0, 'n'},

	/* long options without */
	{"inline", no_argument, &long_option, LONG_OPTION_INLINE},
	{"special", no_argument, &long_option, LONG_OPTION_SPECIAL},

	/* deprecated long opts without */
	{"remove-trailing", no_argument, &long_option, LONG_OPTION_REMOVE_TRAILING},

	/* done */
	{0, 0, 0, 0}
};

#endif

/* *INDENT-OFF* */

char usage_message[] = {
	"usage: detox [-hLnrvV] [-f configfile] [-s sequence]"
#ifdef HAVE_GETOPT_LONG
	" [--dry-run] [--inline] [--special]"
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
#else
	"	-n 		do a dry run (don't actually do anything)\n"
#endif
	"	-r 		be recursive (descend into subdirectories)\n"
#ifdef HAVE_GETOPT_LONG
	"	--remove-trailing (deprecated)\n"
	"			remove trailing _ and - before a period\n"
#endif
	"	-s sequence	choose which sequence to detox with\n"
#ifdef HAVE_GETOPT_LONG
	"	--special	work on links and special files\n"
#endif
	"	-v 		be verbose\n"
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
#ifdef HAVE_GETOPT_LONG
	"	--remove-trailing (deprecated)\n"
	"			remove trailing _ and - before a period\n"
#endif
	"	-s sequence	choose which sequence to detox with\n"
	"	-v 		be verbose\n"
	"	-V 		show the current version\n"
};

/* *INDENT-ON* */

struct detox_options *initialize_main_options(void)
{
	struct detox_options *main_options;

	main_options = malloc(sizeof(struct detox_options));
	if (main_options == NULL) {
		fprintf(stderr, "out of memory: %s\n", strerror(errno));
		return NULL;
	}

	memset(main_options, 0, sizeof(struct detox_options));

	/*
	 * XXX - handle blank strings better
	 */
	main_options->sequence_name = getenv("DETOX_SEQUENCE");

	return main_options;
}

struct detox_options *parse_options_getopt(int argc, char **argv)
{
	int optcode;

	struct detox_options *main_options;

	int i;
	int max = 10;
	char *binname;

	main_options = initialize_main_options();
	if (main_options == NULL) {
		return NULL;
	}

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
				main_options->check_config_file = strdup(optarg);
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
				main_options->sequence_name = strdup(optarg);
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

					case LONG_OPTION_REMOVE_TRAILING:
						main_options->remove_trailing = 1;
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
				long_option = 0;	/* clean up! */
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

	main_options->files = malloc(sizeof(char *) * 10);

	i = 0;
	max = 10;

	if (optind < argc) {
		while (optind < argc) {
                        /* not enough space for the next file and
                           possible ending NULL -> realloc */
			if (i + 2 > max) {
				main_options->files = realloc(main_options->files, sizeof(char *) * (10 + max));
				max += 10;
			}
			main_options->files[i++] = strdup(argv[optind]);

			optind++;
		}

		main_options->files[i] = NULL;
	}
	else if (!main_options->is_inline_mode) {
		printf("%s", !main_options->is_inline_bin ? usage_message :
			usage_message_inline);
		exit(EXIT_FAILURE);
	}

	return main_options;
}

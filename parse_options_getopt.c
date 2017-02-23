/*
 * Copyright (c) 2005-2006, Doug Harple.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of author nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id: parse_options_getopt.c,v 1.8 2006/07/03 16:45:54 purgedhalo Exp $
 *
 */

#include "config.h"

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "detox.h"
#include "config.h"
#include "parse_options_generic.h"

#ifdef HAVE_GETOPT_LONG
#include <getopt.h>
#endif

enum {
	LONG_OPTION_DRY_RUN = 1,
	LONG_OPTION_REMOVE_TRAILING,
	LONG_OPTION_SPECIAL
};

/* expect this to be overwritten! */
static int long_option = 0;

#ifdef HAVE_GETOPT_LONG
static struct option longopts[] = {

	/* long options with equivalents */
	{"help", no_argument, 0, 'h'},
	{"dry-run", no_argument, 0, 'n'},

	/* long options without */
	{"special", no_argument, &long_option, LONG_OPTION_SPECIAL},

	/* deprecated long opts without */
	{"remove-trailing", no_argument, &long_option, LONG_OPTION_REMOVE_TRAILING},

	/* done */
	{0, 0, 0, 0}
};

#endif

struct detox_options *parse_options_getopt(int argc, char **argv)
{
	int optcode;

	struct detox_options *main_options;

	int i;
	int max = 10;

	main_options = initialize_main_options();
	if (main_options == NULL) {
		return NULL;
	}

#ifdef HAVE_GETOPT_LONG
	while ((optcode = getopt_long(argc, argv, "hrvV?Ls:f:n", longopts, NULL)) != -1) {
#else
	while ((optcode = getopt(argc, argv, "hrvV?Ls:f:n")) != -1) {
#endif
		switch (optcode) {
			case 'h':
				printf(usage_message);
				printf("\n");
				printf(help_message);
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
				printf(usage_message);
				exit(EXIT_SUCCESS);

			case 0:
				switch (long_option) {
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
	max = 0;

	if (optind < argc) {
		while (optind < argc) {
			main_options->files[i++] = strdup(argv[optind]);
			if (i > max) {
				main_options->files = realloc(main_options->files, sizeof(char *) * (10 + max));
				max += 10;
			}

			optind++;
		}

		main_options->files[i] = NULL;
	}
	else {
#ifndef INLINE_MODE
		printf(usage_message);
		exit(EXIT_FAILURE);
#endif
	}

	return main_options;
}

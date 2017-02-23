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
 * $Id: parse_options_popt.c,v 1.8 2006/07/03 16:45:54 purgedhalo Exp $
 *
 */

#include "config.h"

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <popt.h>

#include "detox.h"
#include "config.h"
#include "parse_options_generic.h"

static struct poptOption optionsTable[] = {
	{"dry-run", 'n', POPT_ARG_NONE, 0, 'n', NULL, NULL},
	{"special", '\0', POPT_ARG_NONE, 0, '1', NULL, NULL},
	{"remove-trailing", '\0', POPT_ARG_NONE, 0, '2', NULL, NULL},

	{NULL, 'f', POPT_ARG_STRING, 0, 'f', NULL, NULL},
	{NULL, 'L', POPT_ARG_NONE, 0, 'L', NULL, NULL},
	{NULL, 'r', POPT_ARG_NONE, 0, 'r', NULL, NULL},
	{NULL, 's', POPT_ARG_STRING, 0, 's', NULL, NULL},
	{NULL, 'v', POPT_ARG_NONE, 0, 'v', NULL, NULL},
	{NULL, 'V', POPT_ARG_NONE, 0, 'V', NULL, NULL},

	{"help", 'h', POPT_ARG_NONE, 0, 'h', NULL, NULL},
	{NULL, '?', POPT_ARG_NONE, 0, '?', NULL, NULL},

	{NULL, 0, 0, NULL, 0, NULL, NULL}
};

struct detox_options *parse_options_popt(int argc, const char **argv)
{
	struct detox_options *main_options;

	char *work;

	int c;			/* used for argument parsing */
	poptContext optCon;	/* context for parsing command-line options */

	int i;
	int max = 10;

	main_options = initialize_main_options();
	if (main_options == NULL) {
		return NULL;
	}

	optCon = poptGetContext(NULL, argc, argv, optionsTable, 0);
	poptSetOtherOptionHelp(optCon, "[OPTIONS]* [file]");

	if (argc < 2) {
		poptPrintUsage(optCon, stderr, 0);
		exit(EXIT_FAILURE);
	}

	/* Now do options processing, get portname */
	while ((c = poptGetNextOpt(optCon)) >= 0) {
		switch (c) {
			case 'h':
				printf(usage_message);
				printf("\n");
				printf(help_message);
				exit(EXIT_SUCCESS);

			case 'f':
				/*
				 * XXX - free multiple check_config_files
				 */
				main_options->check_config_file = strdup(poptGetOptArg(optCon));
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
				main_options->sequence_name = strdup(poptGetOptArg(optCon));
				break;

			case 'v':
				main_options->verbose++;
				break;

			case 'V':
				printf("%s\n", PACKAGE_STRING);
				exit(EXIT_SUCCESS);

			case '1':
				main_options->special = 1;
				break;

			case '2':
				main_options->remove_trailing = 1;
				break;

			case '?':
				printf(usage_message);
				exit(EXIT_SUCCESS);

		}
	}

	if (c < -1) {
		fprintf(stderr, "%s: %s\n", poptStrerror(c), poptBadOption(optCon, POPT_BADOPTION_NOALIAS));
		exit(EXIT_FAILURE);
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

	while (work = (char *)poptGetArg(optCon)) {
		main_options->files[i++] = strdup(work);
		if (i > max) {
			main_options->files = realloc(main_options->files, sizeof(char *) * (10 + max));
			max += 10;
		}
	}

	main_options->files[i] = NULL;

#ifndef INLINE_MODE
	if (i == 0) {
		fprintf(stderr, usage_message);
		exit(EXIT_FAILURE);
	}
#endif

	return main_options;
}

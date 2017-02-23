/*
 * Copyright (c) 2006, Doug Harple.  All rights reserved.
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
 * $Id: config_file_spoof.c,v 1.4 2006/07/03 16:45:54 purgedhalo Exp $
 *
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "detox.h"
#include "config_file_spoof.h"
#include "clean_string.h"

struct detox_parse_results *spoof_config_file(struct detox_options *main_options)
{
	struct detox_parse_results *ret = NULL;
	struct detox_sequence_list *sequences = NULL;
	struct detox_sequence_entry *work = NULL;

	/*
	 * Initialize return
	 */

	ret = malloc(sizeof(struct detox_parse_results));
	if (ret == NULL) {
		fprintf(stderr, "out of memory: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	memset(ret, 0, sizeof(struct detox_parse_results));

	/*
	 * Head of sequence
	 */

	sequences = malloc(sizeof(struct detox_sequence_list));
	if (sequences == NULL) {
		fprintf(stderr, "out of memory: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	memset(sequences, 0, sizeof(struct detox_sequence_list));

	sequences->name = strdup("default");
	sequences->source_filename = strdup("inside the beast");

	/*
	 * Step 1 - ISO8859_1
	 */

	sequences->head = malloc(sizeof(struct detox_sequence_entry));
	if (sequences->head == NULL) {
		fprintf(stderr, "out of memory: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	work = sequences->head;
	memset(work, 0, sizeof(struct detox_sequence_entry));

	work->cleaner = &clean_iso8859_1;

	/*
	 * Step 2 - Safe
	 */

	work->next = malloc(sizeof(struct detox_sequence_entry));
	if (work->next == NULL) {
		fprintf(stderr, "out of memory: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	work = work->next;
	memset(work, 0, sizeof(struct detox_sequence_entry));

	work->cleaner = &clean_safe;

	/*
	 * Step 3 - Wipe Up
	 */

	work->next = malloc(sizeof(struct detox_sequence_entry));
	if (work->next == NULL) {
		fprintf(stderr, "out of memory: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	work = work->next;
	memset(work, 0, sizeof(struct detox_sequence_entry));

	work->cleaner = &clean_wipeup;

	/*
	 * Deprecated
	 */
	if (main_options->remove_trailing) {
		static struct clean_string_options *csopts;

		csopts = malloc(sizeof(struct clean_string_options));
		if (csopts == NULL) {
			fprintf(stderr, "out of memory: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}

		memset(csopts, 0, sizeof(struct clean_string_options));
		csopts->remove_trailing = 1;

		work->options = csopts;
	}

	/*
	 *
	 */

	ret->sequences = sequences;

	return ret;
}

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
 * $Id: file-inline.c,v 1.2 2006/07/03 16:45:54 purgedhalo Exp $
 *
 */

#include "config.h"

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

#include "clean_string.h"
#include "file-inline.h"
#include "detox.h"

#define BUF_SIZE 1024

/*
 * Renames file to a safe filename.
 */
void parse_inline(unsigned char *filename, struct detox_options *options)
{
	struct detox_sequence_entry *sequence;
	FILE *fp;
	unsigned char *base, *work, *hold;
	int err;
	size_t len;
	size_t buf_size;

	if (filename != NULL) {
		if (!(fp = fopen(filename, "r"))) {
			fprintf(stderr, "%s: %s\n", filename, strerror(errno));
			return;
		}
	}
	else {
		fp = stdin;
	}

	buf_size = BUF_SIZE;
	base = malloc(buf_size);
	if (base == NULL) {
		fprintf(stderr, "out of memory: %s\n", strerror(errno));
		return;
	}

	while (fgets(base, buf_size, fp)) {
		while (strrchr(base, '\n') == NULL) {
			work = realloc(base, buf_size + BUF_SIZE - 1);
			if (!fgets(work + buf_size - 1, BUF_SIZE, fp)) {
				base = work;
				break;
			}
			base = work;
			buf_size += BUF_SIZE - 1;
		}

		hold = strrchr(base, '\n');
		if (hold == NULL) {
			fprintf(stderr, "Unable to parse input\n");
			exit(EXIT_FAILURE);
		}
		*hold = '\0';

		work = strdup(base);

		sequence = options->sequence_to_use;

		while (sequence != NULL && work != NULL) {
			hold = sequence->cleaner(work, sequence->options);
			if (work != NULL) {
				free(work);
			}
			work = hold;

			sequence = sequence->next;
		}

		if (work != NULL) {
			printf("%s\n", work);
		}
		else {
			printf("\n");
		}
	}
}

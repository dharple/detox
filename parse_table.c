/*
 * Copyright (c) 2004-2006, Doug Harple.  All rights reserved.
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
 * $Id: parse_table.c,v 1.10 2006/07/03 16:45:54 purgedhalo Exp $
 *
 */

#include "config.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <locale.h>

#include "table.h"

enum {
	BASE_STATE,
	INSIDE_STATE
};

struct translation_table *parse_table(char *filename)
{
	FILE *ttable_file;
	char *work;
	int code;
	int offset;
	char *parsed;
	int err;
	int size;
	int max_data_length;
	int ret;
	int state;
	char *system_ctype;

	struct translation_table *table;

	struct stat ttable_stat;

	err = stat(filename, &ttable_stat);
	if (err == -1) {
		return NULL;
	}

	system_ctype = setlocale(LC_CTYPE, "");
	if (system_ctype == NULL) {
		system_ctype = "";	// I don't think we can free the return from setlocale()
	}

	if (ttable_stat.st_size > 0) {
		size = ttable_stat.st_size;
	}
	else {
		size = (512 * ttable_stat.st_blocks);
	}

	size /= 6;

	if (size < 500) {
		size = 500;
	}

	table = table_init(size);
	if (table == NULL) {
		return NULL;
	}

	ttable_file = fopen(filename, "r");
	if (ttable_file == NULL) {
		fprintf(stderr, "Unable to open translation table: %s\n", strerror(errno));
		return NULL;
	}

	work = malloc(1024);
	if (work == NULL) {
		fprintf(stderr, "Unable to allocate memory: %s\n", strerror(errno));
		return NULL;
	}

	parsed = malloc(1024);
	if (parsed == NULL) {
		fprintf(stderr, "Unable to allocate memory: %s\n", strerror(errno));
		return NULL;
	}

	max_data_length = 1;
	state = BASE_STATE;

	while (fgets(work, 1024, ttable_file) != NULL) {
		if (*work == '#') {

			/*
			 * Don't even bother
			 */
			continue;
		}

		parsed[0] = '\0';

		if (state == BASE_STATE) {
			ret = sscanf(work, " %s %n", parsed, &offset);

			if (ret == 0) {
				continue;
			}

			if (strncasecmp(parsed, "start", 5) == 0) {
				if (work[offset] == '\0') {
					// All languages
					state = INSIDE_STATE;
					continue;
				}

				if (work[offset] == '"') {
					sscanf(work + offset + 1, "%[^\"]", parsed);
				}
				else if (work[offset] == '\'') {
					sscanf(work + offset + 1, "%[^']", parsed);
				}
				else {
					sscanf(work + offset, "%s", parsed);
				}

				if (strncasecmp(parsed, system_ctype, strlen(parsed)) == 0) {
					state = INSIDE_STATE;
				}
				// else ignore this start/end block

				continue;
			}

			if (strncasecmp(parsed, "default", 7) == 0) {

				if (work[offset] == '\0') {
					table->default_translation = NULL;
					continue;
				}

				if (work[offset] == '"') {
					sscanf(work + offset + 1, "%[^\"]", parsed);
				}
				else if (work[offset] == '\'') {
					sscanf(work + offset + 1, "%[^']", parsed);
				}
				else {
					sscanf(work + offset, "%s", parsed);
				}

				table->default_translation = strdup(parsed);

				if (strlen(parsed) > max_data_length) {
					max_data_length = strlen(parsed);
				}

				continue;
			}

			continue;
		}

		/*
		 * Inside state
		 */

		code = -1;

		ret = sscanf(work, "%i %n", &code, &offset);

		if (ret == 0 || code < 0 || offset < 0) {

			/*
			 * Check for end
			 */
			ret = sscanf(work, " %s %n", parsed, &offset);

			if (ret > 0 && strncasecmp(parsed, "end", 5) == 0) {
				state = BASE_STATE;
			}

			continue;
		}

		if (work[offset] == '\0') {
			continue;
		}

		if (work[offset] == '"') {
			sscanf(work + offset + 1, "%[^\"]", parsed);
		}
		else if (work[offset] == '\'') {
			sscanf(work + offset + 1, "%[^']", parsed);
		}
		else {
			sscanf(work + offset, "%s", parsed);
		}

		table_put(table, code, parsed);

		if (strlen(parsed) > max_data_length) {
			max_data_length = strlen(parsed);
		}

	}

	table->max_data_length = max_data_length;

	free(work);
	free(parsed);
	fclose(ttable_file);

	return table;
}

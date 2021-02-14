/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * Helper utility to review tables.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

#include "table.h"
#include "table_dump.h"

enum {
	BASE_STATE,
	INSIDE_STATE
};

void check_table(char *filename, int dump) {
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
	int last;

	struct translation_table *table;

	struct stat ttable_stat;

	printf("checking: %s\n", filename);

	err = stat(filename, &ttable_stat);
	if (err == -1) {
//		fprintf(stderr, "Unable to stat translation table: %s\n", strerror(errno));
		return;
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
		return;
	}

	ttable_file = fopen(filename, "r");
	if (ttable_file == NULL) {
		fprintf(stderr, "Unable to open translation table: %s\n", strerror(errno));
		return;
	}

	work = malloc(1024);
	if (work == NULL) {
		fprintf(stderr, "Unable to allocate memory: %s\n", strerror(errno));
		return;
	}

	parsed = malloc(1024);
	if (parsed == NULL) {
		fprintf(stderr, "Unable to allocate memory: %s\n", strerror(errno));
		return;
	}

	last = 0;

	max_data_length = 1;
	state = BASE_STATE;

	while(fgets(work, 1024, ttable_file) != NULL) {
		// printf(work);
		if (*work == '#') {
			// 
			// Don't even bother
			//
			continue;
		}

		parsed[0] = '\0';

		if (state == BASE_STATE) {
			ret = sscanf(work, " %s %n", parsed, &offset);

			if (ret == 0) {
				continue;
			}

			if (strncasecmp(parsed, "start", 5) == 0) {
				state = INSIDE_STATE;
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

		//
		// Inside state
		//

		code = -1;

		ret = sscanf(work, "%i %n", &code, &offset);

		if (ret == 0 || code < 0 || offset < 0) {
			//
			// Check for end
			//
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

		if (code < last) {
			printf("warning - code 0x%04X appears after 0x%04X\n", code, last);
		}
		else if (code == last) {
			printf("warning - code 0x%04X appears twice in a row\n", code);
		}
		last = code;
	}

	table->max_data_length = max_data_length;

	free(work);
	free(parsed);
	fclose(ttable_file);

	table_dump(table, !dump);

	return;
}


int main(int argc, char **argv) {
	int optcode;
	int dump = 0;

	while ((optcode = getopt(argc, argv, "d")) != -1) {
		switch (optcode) {
			case 'd':
				dump = 1;
				break;

			case 'h':
				printf("check_table [-d] filename.tbl\n");
				return 0;

			default:
				fprintf(stderr, "unknown option: %c\n", optcode);
				return -1;
		}
	}

	if (optind < argc) {
		check_table(argv[optind], dump);
	}
	else {
		printf("please specify a file to operate on\n");
		return -1;
	}

	return 0;
}

/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * Helper utility that generates conversion tables.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "table.h"
#include "parse_table.h"

enum {
	BASE_STATE,
	INSIDE_STATE
};

void dump_table(struct translation_table *table) {
	char *parsed;
	int i;

	for (i = 0; i < 256; i++) {
		parsed = table_get(table, i);
		if (parsed == NULL) {
			parsed = table->default_translation;
		}

		if (parsed != NULL) {
			printf("\t\"%s\",\t\t\t/* 0x%04X */\n", parsed, i);
		}
	}

	return;
}


int main(int argc, char **argv) {
	struct translation_table *table;

	if (argc == 1) {
		return -1;
	}

	table = parse_table(argv[1]);

	dump_table(table);

	return 0;
}

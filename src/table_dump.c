/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * Helper utility to dump a translation table.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "table_dump.h"

void table_dump(struct translation_table *table, int just_stats) {
	int i;

	if (!just_stats) {
		for (i = 0; i < table->length; i++) {
			printf("index: %d, ", i);

			if (table->rows[i].key == 0) {
				printf("empty\n");
			}
			else {
				printf("key: 0x%04x, data: %s\n", table->rows[i].key, table->rows[i].data);
			}
		}
	}

	printf("used: %d\n", table->used);
	printf("max: %d\n", table->length);
	printf("used percent: %0.2f\n", table->used*100.0/table->length);

	printf("hits: %d\n", table->hits);
	printf("misses: %d\n", table->misses);
	printf("overwrites: %d\n", table->overwrites);

}

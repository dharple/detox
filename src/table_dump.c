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

#include "table_dump.h"

void table_dump(table_t *table, int verbose)
{
    int i;

    for (i = 0; i < table->length; i++) {
        if (!verbose && table->rows[i].key == 0) {
            continue;
        }

        printf("index: %d, ", i);

        if (table->rows[i].key == 0) {
            printf("empty\n");
        } else {
            printf("key: 0x%04x, data: %s\n", table->rows[i].key, table->rows[i].data);
        }
    }
}

void table_stats(table_t *table)
{
    printf("used: %d\n", table->used);
    printf("available: %d\n", table->length);
    printf("used percent: %0.2f\n", table->used * 100.0 / table->length);

    printf("hits: %d\n", table->hits);
    printf("misses: %d\n", table->misses);
    printf("seeks: %d\n", table->seeks);
    printf("overwrites: %d\n", table->overwrites);

    printf("longest entry: %d\n", table->max_data_length);
    printf("default translation: %s\n", table->default_translation);
    printf("maximum key: 0x%04x\n", table->max_key);
}

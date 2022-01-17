/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"

struct test_data {
    int value;
    char *data;
    int max_length;
};

#define DATA_COUNT 13
static struct test_data data[DATA_COUNT] = {
    {
        .value      = 0x0001,
        .data       = "a",
        .max_length = 1
    },
    {
        .value      = 0x0002,
        .data       = "b",
        .max_length = 1
    },
    {
        .value      = 0x0003,
        .data       = "c",
        .max_length = 1
    },
    {
        .value      = 0x0004,
        .data       = "don't",
        .max_length = 5
    },
    {
        .value      = 0x0005,
        .data       = "eat",
        .max_length = 5
    },
    {
        .value      = 0x0006,
        .data       = "the",
        .max_length = 5
    },
    {
        .value      = 0x0007,
        .data       = "joe o's",
        .max_length = 7
    },
    {
        .value      = 0x0008,
        .data       = "k",
        .max_length = 7
    },
    {
        .value      = 0x0009,
        .data       = "moo",
        .max_length = 7
    },
    {
        .value      = 0x000A,
        .data       = "true",
        .max_length = 7
    },
    {
        .value      = 0x000B,
        .data       = "insanity",
        .max_length = 8
    },
    {
        .value      = 0x1001,
        .data       = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed",
        .max_length = 60,
    },
    {
        .value      = 0x1002,
        .data       = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.",
        .max_length = 123,
    },
};

#test test_table_max_length
    table_t *table;
    int i;

    table = table_init(DATA_COUNT * 10);

    for (i = 0; i < DATA_COUNT; i++) {
        table_put(table, data[i].value, data[i].data);
        ck_assert_int_eq(table->max_data_length, data[i].max_length);
    }

#test test_table_max_length_after_resize
    table_t *table;
    table_t *check_table;
    int i, j;
    int max_length, old_max_length;

    table = table_init(DATA_COUNT * 10);

    for (i = 0; i < DATA_COUNT; i++) {
        table_put(table, data[i].value, data[i].data);
    }

    max_length = table->max_data_length;

    //
    // confirm that max length gets copied over to a resized table
    //

    for (j = 0; j < 2; j++) {
        check_table = table_resize(table, table->length, j);
        ck_assert_int_eq(check_table->max_data_length, max_length);
        table_free(check_table);
    }

    //
    // put "a" in all the values, so when we resize, the max length on the new
    // table should be 1.
    //

    old_max_length = max_length;
    max_length = 1;

    for (i = 0; i < DATA_COUNT; i++) {
        table_put(table, data[i].value, "a");
    }

    for (j = 0; j < 2; j++) {
        check_table = table_resize(table, table->length, j);
        ck_assert_int_eq(check_table->max_data_length, max_length);
        table_free(check_table);
    }

    //
    // put "" in all the values, so when we resize, the max length on the new
    // table should be 0.
    //

    max_length = 0;

    for (i = 0; i < DATA_COUNT; i++) {
        table_put(table, data[i].value, "");
    }

    for (j = 0; j < 2; j++) {
        check_table = table_resize(table, table->length, j);
        ck_assert_int_eq(check_table->max_data_length, max_length);
        table_free(check_table);
    }

    //

    ck_assert_int_eq(table->max_data_length, old_max_length);

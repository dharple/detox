/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <string.h>

#include "../../src/clean_string.h"

#include "unit_struct.h"

#define DATA_COUNT 4
static struct test_filename data[DATA_COUNT] = {
    { .filename = "lower",      .expected_a = "lower" },
    { .filename = "L0W3R",      .expected_a = "l0w3r" },
    { .filename = "UPPER",      .expected_a = "upper" },
    { .filename = "UPPer_2",    .expected_a = "upper_2" },
};

#test test_clean_lower
    char *output;
    int i;

    // legacy tests
    for (i = 0; i < DATA_COUNT; i++) {
        output = clean_lower(data[i].filename, NULL);
        ck_assert_str_eq(output, data[i].expected_a);
    }

    // confirm NULL works
    output = clean_lower(NULL, NULL);
    ck_assert(output == NULL);

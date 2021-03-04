/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <string.h>

// need defines first
#include "config.h"

#include "clean_string.h"

#include "unit_struct.h"

#define DATA_COUNT 4
static struct test_filename data[DATA_COUNT] = {

    // legacy tests

    { .filename = "lower",      .expected = "lower" },
    { .filename = "L0W3R",      .expected = "l0w3r" },
    { .filename = "UPPER",      .expected = "upper" },
    { .filename = "UPPer_2",    .expected = "upper_2" },

};

#test test_clean_lower
    char *output;
    int i;

    for (i = 0; i < DATA_COUNT; i++) {
        output = clean_lower(data[i].filename);
        ck_assert_str_eq(output, data[i].expected);
    }

#test test_clean_lower_null
    char *output;

    // confirm NULL works
    output = clean_lower(NULL);
    ck_assert(output == NULL);

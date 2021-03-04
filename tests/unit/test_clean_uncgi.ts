/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <string.h>

#include "clean_string.h"

#include "unit_struct.h"

#define DATA_COUNT 10
static struct test_filename data[DATA_COUNT] = {

    // legacy tests

    { .filename = "%3Dequals",          .expected = "=equals" },
    { .filename = "%25percent",         .expected = "%percent" },
    { .filename = "%3Fquestion%20mark", .expected = "?question mark" },
    { .filename = "%2bplus",            .expected = "+plus" },
    { .filename = "%2ccomma",           .expected = ",comma" },
    { .filename = "%2Fslash",           .expected = "/slash" },

    // full coverage

    { .filename = "lower",              .expected = "lower" },
    { .filename = "%20space",           .expected = " space" },
    { .filename = "UPPER%3ALOWER",      .expected = "UPPER:LOWER" },
    { .filename = "here+and+there",     .expected = "here and there" },

};

#test test_clean_uncgi
    char *output;
    int i;

    for (i = 0; i < DATA_COUNT; i++) {
        output = clean_uncgi(data[i].filename);
        ck_assert_str_eq(output, data[i].expected);
    }

#test test_clean_uncgi_null
    char *output;

    // confirm NULL works
    output = clean_uncgi(NULL);
    ck_assert(output == NULL);

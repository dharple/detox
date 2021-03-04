/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "clean_string.h"
#include "detox_struct.h"

#include "unit_struct.h"

#define DATA_COUNT 11
static struct test_filename data[DATA_COUNT] = {
    {
        .filename   = "-#dash_octothorpe-",
        .expected_a = "dash_octothorpe-",
        .expected_b = "dash_octothorpe-",
    },
    {
        .filename   = "-_dash_underscore-",
        .expected_a = "dash_underscore-",
        .expected_b = "dash_underscore-",
    },
    {
        .filename   = "ends with octothorpe#",
        .expected_a = "ends with octothorpe#",
        .expected_b = "ends with octothorpe#",
    },
    {
        .filename   = "#octothorpe",
        .expected_a = "octothorpe",
        .expected_b = "octothorpe",
    },
    {
        .filename   = "#-octothorpe_dash",
        .expected_a = "octothorpe_dash",
        .expected_b = "octothorpe_dash",
    },
    {
        .filename   = "_-underscore_dash",
        .expected_a = "underscore_dash",
        .expected_b = "underscore_dash",
    },
    {
        .filename   = "_underscore",
        .expected_a = "underscore",
        .expected_b = "underscore",
    },
    {
        .filename = "___________underscore_______________________.x",
        .expected_a = "underscore_.x",
        .expected_b = "underscore.x",
    },
    {
        .filename = "..........how.....does.....this....translate......",
        .expected_a = "..........how.....does.....this....translate......",
        .expected_b = ".how.does.this.translate.",
    },
    {
        .filename = "_-_-_-_-_-dotted-_-_-_-_line.....part......two.......",
        .expected_a = "dotted-line.....part......two.......",
        .expected_b = "dotted-line.part.two.",
    },
    {
        .filename = "blah-.-de_._da",
        .expected_a = "blah-.-de_._da",
        .expected_b = "blah.de.da",
    },
};

#test test_clean_wipeup
    char *output;
    int i;

    // legacy tests
    for (i = 0; i < DATA_COUNT; i++) {
        output = clean_wipeup(data[i].filename, 0);
        ck_assert_str_eq(output, data[i].expected_a);
    }

#test test_clean_wipeup_null
    char *output;

    // confirm NULL works
    output = clean_wipeup(NULL, 0);
    ck_assert(output == NULL);

#test test_clean_wipeuprt
    char *output;
    int i;

    // legacy tests
    for (i = 0; i < DATA_COUNT; i++) {
        output = clean_wipeup(data[i].filename, 1);
        ck_assert_str_eq(output, data[i].expected_b);
    }

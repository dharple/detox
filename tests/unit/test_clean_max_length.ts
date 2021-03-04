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

#define DATA_COUNT 18
static struct test_filename data[DATA_COUNT] = {

    // legacy tests

    {
        .filename   = "___________underscore_______________________.x",
        .expected   = "___________underscore_________.x",
        .max_length = 32,
    },
    {
        .filename   = "..........how.....does.....this....translate......",
        .expected   = "..........how.....does.....this.",
        .max_length = 32,
    },
    {
        .filename   = "_-_-_-_-_-dotted-_-_-_-_line.....part......two.......",
        .expected   = "_-_-_-_-_-dotted-_-_-_-_line....",
        .max_length = 32,
    },
    {
        .filename   = "blah-.-de_._da",
        .expected   = "blah-.-de_._da",
        .max_length = 32,
    },

    // from the detoxrc.5 manpage

    {
        .filename   = "this_is_my_file.txt",
        .expected   = "this_is_.txt",
        .max_length = 12,
    },

    // github issue 46

    {
        .filename   = "safe.txt",
        .expected   = "safe.txt",
        .max_length = 32,
    },

    {
        .filename   = "safe.and.stu.ff.txt",
        .expected   = "safe.an.ff.txt",
        .max_length = 14,
    },

    {
        .filename   = "safe and stuff.txt",
        .expected   = "safe and.txt",
        .max_length = 12,
    },

    {
        .filename   = "safe and stuff.txt",
        .expected   = "safe.txt",
        .max_length = 8,
    },

    {
        .filename   = "safe.tar.gz",
        .expected   = "safe.tar.gz",
        .max_length = 32,
    },

    {
        .filename   = "safe and stuff.tar.gz",
        .expected   = "safe and stuff.tar.gz",
        .max_length = 22,
    },

    {
        .filename   = "safe and stuff.tar.gz",
        .expected   = "safe and stuff.tar.gz",
        .max_length = 21,
    },

    {
        .filename   = "safe and stuff.tar.gz",
        .expected   = "safe and stuf.tar.gz",
        .max_length = 20,
    },

    {
        .filename   = "safe and stuff.tar.gz",
        .expected   = "safe .tar.gz",
        .max_length = 12,
    },

    {
        .filename   = "safe and stuff.tar.gz",
        .expected   = "sa.tar.gz",
        .max_length = 9,
    },

    {
        .filename   = "safe and stuff.tar.gz",
        .expected   = "s.tar.gz",
        .max_length = 8,
    },

    {
        .filename   = "safe and stuff.tar.gz",
        .expected   = "safe and stuff.tar.gz",
        .max_length = 7,
    },

    {
        .filename   = "safe and stuff.tar.gz",
        .expected   = "safe and stuff.tar.gz",
        .max_length = 0,
    },
};

#test test_clean_max_length
    char *output;
    int i;

    for (i = 0; i < DATA_COUNT; i++) {
        output = clean_max_length(data[i].filename, data[i].max_length);
        ck_assert_str_eq(output, data[i].expected);
    }

#test test_clean_max_length_null
    char *output;

    // confirm NULL works
    output = clean_max_length(NULL, 0);
    ck_assert(output == NULL);

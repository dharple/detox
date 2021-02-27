/*
 * DO NOT EDIT THIS FILE. Generated by checkmk.
 * Edit the original source file "test_clean_wipeup.ts" instead.
 */

#include <check.h>

#line 1 "test_clean_wipeup.ts"
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

#include "../../src/clean_string.h"
#include "../../src/detox_struct.h"

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

START_TEST(test_clean_wipeup)
{
#line 79
    char *output;
    int i;

    // legacy tests
    for (i = 0; i < DATA_COUNT; i++) {
        output = clean_wipeup(data[i].filename, NULL);
        ck_assert_str_eq(output, data[i].expected_a);
    }

    // confirm NULL works
    output = clean_wipeup(NULL, NULL);
    ck_assert(output == NULL);

}
END_TEST

START_TEST(test_clean_wipeuprt)
{
#line 93
    struct clean_string_options *options;
    char *output;
    int i;

    // Allocate an options struct
    options = malloc(sizeof(struct clean_string_options));
    ck_assert_msg((options != NULL), "options could not be allocated");
    memset(options, 0, sizeof(struct clean_string_options));
    options->remove_trailing = 1;

    // legacy tests
    for (i = 0; i < DATA_COUNT; i++) {
        output = clean_wipeup(data[i].filename, options);
        ck_assert_str_eq(output, data[i].expected_b);
    }
}
END_TEST

int main(void)
{
    Suite *s1 = suite_create("Core");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc1_1);
    tcase_add_test(tc1_1, test_clean_wipeup);
    tcase_add_test(tc1_1, test_clean_wipeuprt);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
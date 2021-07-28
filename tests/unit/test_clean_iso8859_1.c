/*
 * DO NOT EDIT THIS FILE. Generated by checkmk.
 * Edit the original source file "test_clean_iso8859_1.ts" instead.
 */

#include <check.h>

#line 1 "test_clean_iso8859_1.ts"
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

#include "builtin_table.h"
#include "clean_string.h"
#include "detox_struct.h"
#include "table.h"

#include "unit_struct.h"

#define DATA_COUNT 6
static struct test_filename data[DATA_COUNT] = {

    // legacy tests

    {
        .filename   = "safe string",
        .expected_a = "safe string",
        .expected_b = "safe string",
    },
    {
        .filename = "\xAE reg",
        .expected_a = "_reg_ reg",
        .expected_b = "_reg_ reg",
    },
    {
        .filename   = "\xA9 copy",
        .expected_a = "_copy_ copy",
        .expected_b = "_copy_ copy",
    },
    {
        .filename   = "\xC6 capital AE",
        .expected_a = "AE capital AE",
        .expected_b = "AE capital AE",
    },
    {
        .filename   = "\xDE capital thorn",
        .expected_a = "TH capital thorn",
        .expected_b = "TH capital thorn",
    },

    // full coverage - test default translation rules

    {
        .filename   = "\x80 not in table",
        .expected_a = "_ not in table",
        .expected_b = "\x80 not in table",
    },
};

START_TEST(test_clean_iso8859_1)
{
#line 62
    table_t *table_a;
    table_t *table_b;
    char *output;
    int i;

    table_a = load_builtin_iso8859_1_table();
    table_b = load_builtin_iso8859_1_table();

    table_a->default_translation = strdup("_");
    table_b->default_translation = NULL;

    // confirm that our a/b test is still valid
    ck_assert(table_get(table_b, 0x80) == NULL);

    for (i = 0; i < DATA_COUNT; i++) {
        output = clean_iso8859_1(data[i].filename, table_a);
        ck_assert_str_eq(output, data[i].expected_a);

        output = clean_iso8859_1(data[i].filename, table_b);
        ck_assert_str_eq(output, data[i].expected_b);
    }

}
END_TEST

START_TEST(test_clean_iso8859_1_null)
{
#line 85
    char *output;

    // confirm NULL works
    output = clean_iso8859_1(NULL, NULL);
    ck_assert(output == NULL);

}
END_TEST

START_TEST(test_clean_iso8859_1_missing_table)
{
#line 92
    clean_iso8859_1("what", NULL);
}
END_TEST

int main(void)
{
    Suite *s1 = suite_create("Core");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc1_1);
    tcase_add_test(tc1_1, test_clean_iso8859_1);
    tcase_add_test(tc1_1, test_clean_iso8859_1_null);
    tcase_add_exit_test(tc1_1, test_clean_iso8859_1_missing_table, 1);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
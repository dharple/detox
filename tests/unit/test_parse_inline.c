/*
 * DO NOT EDIT THIS FILE. Generated by checkmk.
 * Edit the original source file "test_parse_inline.template" instead.
 * Run `make internals` from the base of the project to regenerate this file.
 */

#include <check.h>

#line 1 "test_parse_inline.template"
/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "builtin_table.h"
#include "detox_struct.h"
#include "file.h"
#include "filter.h"
#include "parse_options.h"
#include "sequence.h"
#include "table.h"
#include "wrapped.h"

#define LOCAL_PADDING 32

struct test_values {
    int table_value;
    char *table_entry;

    char *input;
    char *expected;
};

#define DATA_COUNT 9
static struct test_values data[DATA_COUNT] = {
    // 4 byte UTF-8
    {
        .table_value    = 0x1F37A,
        .table_entry    = "_root_beer_",

        .input          = "\U0001F37A",
        .expected       = "_root_beer_",
    },

    // 3 byte UTF-8
    {
        .table_value    = 0x2021,
        .table_entry    = "_double_dagger_",

        .input          = "\u2021",
        .expected       = "_double_dagger_",
    },

    // 2 byte UTF-8
    {
        .table_value    = 0x00AE,
        .table_entry    = "(r)",

        .input          = "\u00AE",
        .expected       = "(r)",
    },

    // invalid 4 byte UTF-8
    {
        .table_value    = 0x0001,
        .table_entry    = "never...",

        .input          = "\xF0" "\x9F" "\x8D",
        .expected       = "_",
    },

    // invalid 4 byte UTF-8 with more text
    {
        .table_value    = 0x0002,
        .table_entry    = "never...",

        .input          = "\xF0" "\x9F" "\x8D" "whatever",
        .expected       = "_whatever",
    },

    // invalid 3 byte UTF-8
    {
        .table_value    = 0x0003,
        .table_entry    = "never...",

        .input          = "\xE2" "\x80",
        .expected       = "_",
    },

    // invalid 3 byte UTF-8 with more text
    {
        .table_value    = 0x0004,
        .table_entry    = "never...",

        .input          = "\xE2" "\x80" "whatever",
        .expected       = "_whatever",
    },

    // invalid 2 byte UTF-8
    {
        .table_value    = 0x0005,
        .table_entry    = "never...",

        .input          = "\xC0",
        .expected       = "_",
    },

    // invalid 2 byte UTF-8 with more text
    {
        .table_value    = 0x0006,
        .table_entry    = "never...",

        .input          = "\xC0" "whatever",
        .expected       = "_whatever",
    },
};

#define BAD_FILES_DATA_COUNT 6
static struct test_values bad_files_data[BAD_FILES_DATA_COUNT] = {
    {
        .input          = "..",
        .expected       = "..",
    },

    {
        .input          = ".git",
        .expected       = ".git",
    },

    {
        .input          = ".UPPER",
        .expected       = ".upper",
    },

    {
        .input          = "not.UPPER",
        .expected       = "not.upper",
    },

    {
        .input          = ".wipeup------no",
        .expected       = ".wipeup-no",
    },

    {
        .input          = "wipeup------yes",
        .expected       = "wipeup-yes",
    },
};

START_TEST(test_parse_inline)
{
#line 154
    FILE *in_fp;
    FILE *out_fp;

    options_t *options;
    sequence_t *sequence;
    filter_t *filter;

    char *expected;
    char *input;
    char *input_filename;
    char *output;
    char *output_filename;
    char *walk;
    int buffer_size;
    int end;
    int start;
    int use_newline;
    int x_length;
    int half_x_length;
    int i;

    //

    options = options_init();

    options->sequence_to_use = sequence = sequence_init("utf_8-and-lower");
    sequence->source_filename = wrapped_strdup(__FILE__);

    sequence->filters = filter = filter_init(FILTER_UTF_8);
    filter->builtin = wrapped_strdup("safe");
    filter->table = load_builtin_safe_table();

    for (i = 0; i < DATA_COUNT; i++) {
        // table_put(filter->table, 0x0001F37A, "_root_beer_");
        table_put(filter->table, data[i].table_value, data[i].table_entry);
    }

    filter->next = filter_init(FILTER_LOWER);

    //

    start = INLINE_BUF_SIZE - (INLINE_BUF_PADDING * 2);
    end = INLINE_BUF_SIZE + (INLINE_BUF_PADDING * 2);
    buffer_size = end + LOCAL_PADDING;

    expected = wrapped_malloc(buffer_size);
    input    = wrapped_malloc(buffer_size);
    output   = wrapped_malloc(buffer_size);

    input_filename  = wrapped_malloc(buffer_size);
    output_filename = wrapped_malloc(buffer_size);

    for (i = 0; i < DATA_COUNT; i++) {
        for (x_length = start; x_length < end; x_length++) {
            for (use_newline = 0; use_newline < 2; use_newline++) {
#ifdef DEBUG
                fprintf(stderr, "CHECKING VALUE: 0x%04X, LENGTH: %d, NEW LINE: %s\n", data[i].table_value, x_length, use_newline ? "yes" : "no");
#endif

                // prep

                sprintf(input_filename,  "/tmp/detoxtest-in-%d-%d-XXXXXX", x_length, use_newline);
                sprintf(output_filename, "/tmp/detoxtest-out-%d-%d-XXXXXX", x_length, use_newline);

                mktemp(input_filename);
                mktemp(output_filename);

#ifdef DEBUG
                fprintf(stderr, "in: %s, out: %s\n", input_filename, output_filename);
#endif

                // wipe memory

                memset(expected, 0, buffer_size);
                memset(input,    0, buffer_size);
                memset(output,   0, buffer_size);

                // ---------------------------------------------------------
                // BUILD INPUT
                // ---------------------------------------------------------

                // set input to string filled with the same character, "x".
                // the length is x_length.

                memset(input, 'x', x_length);

                // stick a bunch of uppercase Xs in the middle

                half_x_length = (int)(x_length / 2);
                memset(input + (int)(half_x_length / 2), 'X', half_x_length);

                // at the end of the input, add a 4-byte UTF-8 character

                walk = input + x_length;
                sprintf(walk, "%s%s", data[i].input, use_newline ? "\n" : "");

                // ---------------------------------------------------------
                // BUILD EXPECTED
                // ---------------------------------------------------------

                // set expected to string filled with the same character, "x".
                // the length is x_length.

                memset(expected, 'x', x_length);

                // at the end of the expected, add the translated character

                walk = expected + x_length;
                sprintf(walk, "%s%s", data[i].expected, use_newline ? "\n" : "");

                // ---------------------------------------------------------

                // write the string to a file

                in_fp = fopen(input_filename, "w");
                fputs(input, in_fp);
                fclose(in_fp);

                // pass the file to parse_inline

                parse_inline(input_filename, output_filename, options);

                // read the output file

                out_fp = fopen(output_filename, "r");
                if (fgets(output, buffer_size, out_fp) == NULL) {
                    ck_abort_msg("fgets() failed");
                }
                fclose(out_fp);

                // compare

                ck_assert_str_eq(expected, output);

                // cleanup

                unlink(output_filename);
                unlink(input_filename);

            }
        }
    }

}
END_TEST

START_TEST(test_parse_inline_protected_file)
{
#line 298
    FILE *in_fp;
    FILE *out_fp;

    options_t *options;
    sequence_t *sequence;
    filter_t *filter;

    char *expected;
    char *input;
    char *input_filename;
    char *output;
    char *output_filename;
    char *walk;
    int buffer_size;
    int i;

    //

    options = options_init();

    options->sequence_to_use = sequence = sequence_init("wipeup-lower");
    sequence->source_filename = wrapped_strdup(__FILE__);

    sequence->filters = filter = filter_init(FILTER_WIPEUP);
    filter->remove_trailing = 1;

    filter->next = filter_init(FILTER_LOWER);

    //

    buffer_size = INLINE_BUF_SIZE * 2;

    expected = wrapped_malloc(buffer_size);
    input    = wrapped_malloc(buffer_size);
    output   = wrapped_malloc(buffer_size);

    input_filename  = wrapped_malloc(buffer_size);
    output_filename = wrapped_malloc(buffer_size);

    // prep

    sprintf(input_filename,  "/tmp/detoxtest-in--XXXXXX");
    sprintf(output_filename, "/tmp/detoxtest-out-XXXXXX");

    mktemp(input_filename);
    mktemp(output_filename);

#ifdef DEBUG
    fprintf(stderr, "in: %s, out: %s\n", input_filename, output_filename);
#endif

    // wipe memory

    memset(expected, 0, buffer_size);
    memset(input,    0, buffer_size);
    memset(output,   0, buffer_size);

    // ---------------------------------------------------------
    // BUILD INPUT
    // ---------------------------------------------------------

    walk = input;
    for (i = 0; i < BAD_FILES_DATA_COUNT; i++) {
        sprintf(walk, "%s\n", bad_files_data[i].input);
        walk = strchr(walk, '\0');
    }

    // ---------------------------------------------------------
    // BUILD EXPECTED
    // ---------------------------------------------------------

    walk = expected;
    for (i = 0; i < BAD_FILES_DATA_COUNT; i++) {
        sprintf(walk, "%s\n", bad_files_data[i].expected);
        walk = strchr(walk, '\0');
    }

    // ---------------------------------------------------------

    // write the string to a file

    in_fp = fopen(input_filename, "w");
    fputs(input, in_fp);
    fclose(in_fp);

    // pass the file to parse_inline

    parse_inline(input_filename, output_filename, options);

    // read the output file

    out_fp = fopen(output_filename, "r");
    walk = output;
    while (fgets(walk, buffer_size, out_fp) != NULL) {
        walk += strlen(walk);
    }
    fclose(out_fp);

    // compare

    ck_assert_str_eq(expected, output);

    // cleanup

    unlink(output_filename);
    unlink(input_filename);
}
END_TEST

int main(void)
{
    Suite *s1 = suite_create("Core");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc1_1);
    tcase_add_test(tc1_1, test_parse_inline);
    tcase_add_test(tc1_1, test_parse_inline_protected_file);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}

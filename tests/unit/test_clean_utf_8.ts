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
#include "clean_utf_8.h"
#include "detox_struct.h"
#include "table.h"

#include "unit_struct.h"

#define DATA_COUNT 19
static struct test_filename data[DATA_COUNT] = {
    // legacy

    {
        .filename   = "safe string",
        .expected_a = "safe string",
        .expected_b = "safe string",
    },
    {
        .filename   = "\u00AE reg",
        .expected_a = "_reg_ reg",
        .expected_b = "_reg_ reg",
    },
    {
        .filename   = "\u00A9 copy",
        .expected_a = "_copy_ copy",
        .expected_b = "_copy_ copy",
    },
    {
        .filename   = "\u00C6 capital AE",
        .expected_a = "AE capital AE",
        .expected_b = "AE capital AE",
    },
    {
        .filename   = "\u00DE capital thorn",
        .expected_a = "TH capital thorn",
        .expected_b = "TH capital thorn",
    },
    {
        .filename   = "\u014A capital ENG",
        .expected_a = "NG capital ENG",
        .expected_b = "NG capital ENG",
    },
    {
        .filename   = "\u0172 capital U with Ogonek",
        .expected_a = "U capital U with Ogonek",
        .expected_b = "U capital U with Ogonek",
    },

    // github issue 40
    // + full coverage - test default translation rules

    {
        .filename   = "_ () whatever",
        .expected_a = "_ () whatever",
        .expected_b = "_ () whatever",
    },
    {
        .filename   = "\u4000 ignore",
        .expected_a = "_ ignore",
        .expected_b = "\u4000 ignore",
    },
    {
        .filename   = "\x7F delete",
        .expected_a = "_ delete",
        .expected_b = "\x7F delete",
    },

    // 2 byte UTF-8 encoded 0x00 (null)
    {
        .filename   = "\xC0\x80 null",
        .expected_a = "_hidden_null_ null",
        .expected_b = "_hidden_null_ null",
    },

    // 2 byte UTF-8 encoded 0x20 (space)
    {
        .filename   = "\xC0\xA0 space",
        .expected_a = "  space",
        .expected_b = "  space",
    },

    // 2 byte UTF-8 encoded 0x30 (zero)
    {
        .filename   = "\xC0\xB0 zero",
        .expected_a = "0 zero",
        .expected_b = "0 zero",
    },

    // 2 byte UTF-8 encoded 0x7E (tilde)
    {
        .filename   = "\xC1\xBE tilde",
        .expected_a = "~ tilde",
        .expected_b = "~ tilde",
    },

    // 2 byte UTF-8 encoded 0x7F (tilde)
    {
        .filename   = "\xC1\xBF delete",
        .expected_a = "_ delete",
        .expected_b = "\xC1\xBF delete",
    },

    // github issue 33

    {
        .filename   = "\U00010348 hwair",
        .expected_a = "hu hwair",
        .expected_b = "hu hwair",
    },

    // confirm invalid single-byte character sequence

    {
        .filename   = "\x80 delete",
        .expected_a = "_ delete",
        .expected_b = "_ delete",
    },

    // phishing

    {
        .filename   = "\u03A1a\u200E\u200EyPa\u1963\u200E : Y\u200E\u200Eo\u1959r \u200Ea\u1974\u1974o\u200E\u1959\u1952t \u200Eis \u1974\u1959\u200E\u200Err\u1971\u200E\u200E\u1952t\u1963y\u200E \u1963o\u200E\u200E\u1974k\u1971d\u200E\u200E for \u200Es\u1971\u1974\u1959\u200E\u200Erity \u200Er\u1971as\u200E\u200Eo\u1952s. P\u200E\u200E\u1963\u1971a\u200Es\u1971 \u1974\u1963\u200E\u200Ei\u1974k \u200E\u200Eo\u1952 t\u200Eh\u1971 \u1963i\u200E\u200E\u1952k b\u1971\u200E\u200E\u1963ow t\u200Eo \u1959\u1952\u1963\u200E\u200Eo\u1974k \u200Eyo\u200E\u1959r a\u200E\u200E\u1974\u1974o\u200E\u1959\u1952t",
        .expected_a = "PayPal : Your account is currently locked for security reasons. Please click on the link below to unlock your account",
        .expected_b = "PayPal : Your account is currently locked for security reasons. Please click on the link below to unlock your account",
    },

    {
        .filename   = "C\u200E\u200EAS\u200EE \u200EID :\u200E E",
        .expected_a = "CASE ID : E",
        .expected_b = "CASE ID : E",
    },
};

#test test_clean_utf_8
    table_t *table_a;
    table_t *table_b;
    char *output;
    int i;

    table_a = load_builtin_unicode_table();
    table_b = load_builtin_unicode_table();

    table_a->default_translation = strdup("_");
    table_b->default_translation = NULL;

    // confirm that our a/b test is still valid
    ck_assert(table_get(table_b, 0x4000) == NULL);
    ck_assert(table_get(table_b, 0x007F) == NULL);

    // legacy tests
    for (i = 0; i < DATA_COUNT; i++) {
        output = clean_utf_8(data[i].filename, table_a);
        ck_assert_str_eq(output, data[i].expected_a);

        output = clean_utf_8(data[i].filename, table_b);
        ck_assert_str_eq(output, data[i].expected_b);
    }

#test test_clean_utf_8_null
    char *output;

    // confirm NULL works
    output = clean_utf_8(NULL, NULL);
    ck_assert(output == NULL);

#test-exit(1) test_clean_utf_8_missing_table
    clean_utf_8("what", NULL);

#test test_clean_utf_8_invalid
    table_t *table;
    char *output;

    table = load_builtin_unicode_table();
    table->default_translation = strdup("_");

    // test an invalid UTF-8 sequence
    // the cleaner should replace the invalid sequence with an underscore

    output = clean_utf_8("\xC0" "blah", table);
    ck_assert_str_eq(output, "_blah");

    output = clean_utf_8("blah" "\xC0", table);
    ck_assert_str_eq(output, "blah_");

#test test_clean_utf_8_beyond_unicode_max
    table_t *table;
    char *output;

    table = load_builtin_unicode_table();
    table->default_translation = strdup("_");

    // 3-byte encoding a lower ASCII "6"

    output = clean_utf_8("\xE0\x80\xB6" "blah", table);
    ck_assert_str_eq(output, "6blah");

    // 4-byte encoding a lower ASCII "6"

    output = clean_utf_8("\xF0\x80\x80\xB6" "blah", table);
    ck_assert_str_eq(output, "6blah");

    // 5-byte encoding a lower ASCII "6"

    output = clean_utf_8("\xF8\x80\x80\x80\xB6" "blah", table);
    ck_assert_str_eq(output, "6blah");

    // 6-byte encoding a lower ASCII "6"

    output = clean_utf_8("\xF8\x80\x80\x80\xB6" "blah", table);
    ck_assert_str_eq(output, "6blah");

    // 6-byte max on Ubuntu 20.04

    // Unicode 0x7FFFFFFF
    output = clean_utf_8("\xFD\xBF\xBF\xBF\xBF\xBF" "blah", table);
    ck_assert_str_eq(output, "_blah");

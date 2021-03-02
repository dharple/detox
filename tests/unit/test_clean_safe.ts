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

#define DATA_COUNT 44
static struct test_filename data[DATA_COUNT] = {
    // legacy

    { .filename = "lower",                      .expected = "lower" },
    { .filename = "^acute",                     .expected = "^acute" },
    { .filename = "&ampersand",                 .expected = "_and_ampersand" },
    { .filename = "<angle bracket left",        .expected = "_angle_bracket_left" },
    { .filename = ">angle bracket right",       .expected = "_angle_bracket_right" },
    { .filename = "\\back slash",               .expected = "_back_slash" },
    { .filename = "`back tick",                 .expected = "_back_tick" },
    { .filename = "|bar",                       .expected = "_bar" },
    { .filename = "{brace left",                .expected = "-brace_left" },
    { .filename = "}brace right",               .expected = "-brace_right" },
    { .filename = ":colon",                     .expected = "_colon" },
    { .filename = ",comma",                     .expected = ",comma" },
    { .filename = "@commercial at",             .expected = "_commercial_at" },
    { .filename = "dash-",                      .expected = "dash-" },
    { .filename = "#dash_octothorpe-",          .expected = "#dash_octothorpe-" },
    { .filename = "_dash_underscore-",          .expected = "_dash_underscore-" },
    { .filename = "$dollar sign",               .expected = "_dollar_sign" },
    { .filename = "\"double quote",             .expected = "_double_quote" },
    { .filename = "ends with octothorpe#",      .expected = "ends_with_octothorpe#" },
    { .filename = "!exclamation point",         .expected = "_exclamation_point" },
    { .filename = "#octothorpe",                .expected = "#octothorpe" },
    { .filename = "#-octothorpe_dash",          .expected = "#-octothorpe_dash" },
    { .filename = "(parenthesis left",          .expected = "-parenthesis_left" },
    { .filename = ")parenthesis right",         .expected = "-parenthesis_right" },
    { .filename = "%percent",                   .expected = "%percent" },
    { .filename = "+plus",                      .expected = "+plus" },
    { .filename = "?question mark",             .expected = "_question_mark" },
    { .filename = ";semi colon",                .expected = "_semi_colon" },
    { .filename = "'single quote",              .expected = "_single_quote" },
    { .filename = " space",                     .expected = "_space" },
    { .filename = "[square bracket left",       .expected = "-square_bracket_left" },
    { .filename = "]square bracket right",      .expected = "-square_bracket_right" },
    { .filename = "_-underscore_dash",          .expected = "_-underscore_dash" },
    { .filename = "_underscore",                .expected = "_underscore" },
    { .filename = "~tilde",                     .expected = "~tilde" },

    // other

    { .filename = "\u00C6 capital AE",          .expected = "\u00C6_capital_AE" },
    { .filename = "\xC6 capital AE",            .expected = "\xC6_capital_AE" },
    { .filename = "\u00DE capital thorn",       .expected = "\u00DE_capital_thorn" },
    { .filename = "\xDE capital thorn",         .expected = "\xDE_capital_thorn" },
    { .filename = "\u0172 capital U Ogonek",    .expected = "\u0172_capital_U_Ogonek" },
    { .filename = "\x7E tilde",                 .expected = "~_tilde" },
    { .filename = "\x7F delete",                .expected = "__delete" },

    // other, with a/b rules

    {
        .filename   = "\x07 bell",
        .expected   = "__bell",
        .expected_b = "_beep__bell",
    },
    {
        .filename   = "\x09 tab \x0A new line",
        .expected   = "__tab___new_line",
        .expected_b = "_tab__tab__nl__new_line",
    },
};

#test test_clean_safe
    table_t *table;
    char *output;
    int i;

    table = load_builtin_safe_table();

    // legacy tests
    for (i = 0; i < DATA_COUNT; i++) {
        output = clean_safe(data[i].filename, table);
        ck_assert_str_eq(output, data[i].expected);
    }

    // confirm NULL works
    output = clean_safe(NULL, NULL);
    ck_assert(output == NULL);

#test-exit(1) test_clean_safe_missing_table
    clean_safe("what", NULL);

#test test_clean_safe_custom
    table_t *table;
    char *output;
    int i;

    table = load_builtin_safe_table();

    table_put(table, 0x07, "_beep_");
    table_put(table, 0x09, "_tab_");
    table_put(table, 0x0a, "_nl_");

    // legacy tests
    for (i = 0; i < DATA_COUNT; i++) {
        output = clean_safe(data[i].filename, table);
        ck_assert_str_eq(output, (data[i].expected_b != NULL) ? data[i].expected_b : data[i].expected);
    }

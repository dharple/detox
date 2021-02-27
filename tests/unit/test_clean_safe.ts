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

#include "../../src/builtin_table.h"
#include "../../src/clean_string.h"
#include "../../src/detox_struct.h"

#include "unit_struct.h"

#define DATA_COUNT 35
static struct test_filename data[DATA_COUNT] = {
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
};

#test test_clean_safe
    struct clean_string_options *options;
    char *output;
    int i;

    options = new_clean_string_options();
    options->translation_table = load_builtin_safe_table();

    // legacy tests
    for (i = 0; i < DATA_COUNT; i++) {
        output = clean_safe(data[i].filename, options);
        ck_assert_str_eq(output, data[i].expected);
    }

#test-exit(1) test_clean_safe_missing_options
    clean_safe("what", NULL);

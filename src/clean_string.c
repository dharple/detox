/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "clean_string.h"
#include "parse_table.h"
#include "table.h"

#define ISO8859_1_UPPER_BIT 0x80

/*
 * Translates ISO 8859-1 characters (Latin-1) into lower ASCII characters.
 */
char *clean_iso8859_1(char *s, void *opts)
{
    char *output, *input_walk, *output_walk, *replace_walk;
    int new_value;

    struct translation_table *table = NULL;
    struct clean_string_options *options = NULL;

    if (s == NULL) {
        return NULL;
    }

    if (opts == NULL) {
        fprintf(stderr, "this shouldn't happen\n");
        exit(EXIT_FAILURE);
    }

    options = (struct clean_string_options *) opts;
    table = options->translation_table;

    output = malloc((strlen(s) * table->max_data_length) + 1);
    if (output == NULL) {
        fprintf(stderr, "out of memory: %s\n", strerror(errno));
        return NULL;
    }

    input_walk = s;
    output_walk = output;

    while (*input_walk != '\0') {
        if ((*input_walk & ISO8859_1_UPPER_BIT) == 0) {
            *output_walk++ = *input_walk++;
            continue;
        }

        new_value = (unsigned char) input_walk[0];

        replace_walk = table_get(table, new_value);

        if (replace_walk == NULL) {
            if (table->default_translation == NULL) {
                /*
                 * Null translation == leave it alone
                 */
                *output_walk++ = *input_walk++;
                continue;
            } else {
                replace_walk = table->default_translation;
            }
        }

        while (*replace_walk != '\0') {
            *output_walk++ = *replace_walk++;
        }

        input_walk++;
    }

    *output_walk = '\0';

    return output;
}

/*
 * Translates unsafe characters using a translation table.
 */
char *clean_safe(char *s, void *opts)
{
    char *output, *input_walk, *output_walk, *replace_walk;

    struct translation_table *table = NULL;
    struct clean_string_options *options = NULL;

    if (s == NULL) {
        return NULL;
    }

    if (opts == NULL) {
        fprintf(stderr, "this shouldn't happen\n");
        exit(EXIT_FAILURE);
    }

    options = (struct clean_string_options *) opts;
    table = options->translation_table;

    output = malloc((strlen(s) * table->max_data_length) + 1);
    if (output == NULL) {
        fprintf(stderr, "out of memory: %s\n", strerror(errno));
        return NULL;
    }

    input_walk = s;
    output_walk = output;

    while (*input_walk != '\0') {
        replace_walk = table_get(table, *input_walk);
        if (replace_walk == NULL) {
            if (table->default_translation == NULL) {

                /*
                 * Null translation == leave it alone
                 */
                *output_walk++ = *input_walk++;
                continue;
            } else {
                replace_walk = table->default_translation;
            }
        }

        while (*replace_walk != '\0') {
            *output_walk++ = *replace_walk++;
        }

        input_walk++;
    }

    *output_walk = '\0';

    return output;
}


/*
 * Cleans up any CGI encoded characters, in the form "%" followed by 2 hex
 * digits.
 */
char *clean_uncgi(char *s, void *opts)
{
    char *output, *input_walk, *output_walk;
    char conv[3];

    if (s == NULL) {
        return NULL;
    }

    output = malloc(strlen(s) + 1);
    if (output == NULL) {
        fprintf(stderr, "out of memory: %s\n", strerror(errno));
        return NULL;
    }

    input_walk = s;
    output_walk = output;

    while (*input_walk != '\0') {
        if (input_walk[0] == '%' && isxdigit(input_walk[1]) && isxdigit(input_walk[2])) {
            conv[0] = input_walk[1];
            conv[1] = input_walk[2];
            conv[2] = 0;
            *output_walk++ = (char) strtol(conv, NULL, 16);
            input_walk += 3;
        } else {
            *output_walk++ = *input_walk++;
        }
    }

    *output_walk = '\0';

    return output;
}


/*
 * Reduces any series of "_" and "-" to a single character.  "-" takes
 * precedence.
 *
 * If "remove_trailing" is set to non-zero, then "." is added to the
 * comparison, and takes precedence.  This has the effect of reducing "-." or
 * "._", etc, to ".".
 *
 * Strips any "-", "_" or "#" from the beginning of a string.
 *
 */
char *clean_wipeup(char *s, void *opts)
{
    char *output, *input_walk, *output_walk;
    int matched;
    int remove_trailing;

    if (s == NULL) {
        return NULL;
    }

    remove_trailing = 0;
    if (opts != NULL) {
        remove_trailing = ((struct clean_string_options *) opts)->remove_trailing;
    }

    /* remove any -, _, or # at beginning of string */
    while (*s == '-' || *s == '_' || *s == '#') {
        s++;
    }

    output = malloc(strlen(s) + 1);
    if (output == NULL) {
        fprintf(stderr, "out of memory: %s\n", strerror(errno));
        return NULL;
    }

    input_walk = s;
    output_walk = output;
    matched = 0;

    while (*input_walk != '\0') {
        switch (*input_walk) {
            case '-':
                if (matched) {
                    if (*output_walk == '_') {
                        *output_walk = '-';
                    }
                } else {
                    *output_walk = '-';
                }

                matched = 1;
                break;

            case '_':
                if (!matched) {
                    *output_walk = '_';
                }

                matched = 1;
                break;

            case '.':
                if (remove_trailing) {
                    *output_walk = '.';
                    matched = 1;
                    break;
                }
            /* else fall through */
            default:
                if (matched) {
                    output_walk++;
                    matched = 0;
                }

                *output_walk++ = *input_walk;
        }
        input_walk++;
    }

    if (matched) {
        output_walk++;
    }

    *output_walk = '\0';

    return output;
}

#define UTF_8_ENCODED 0x80
#define UTF_8_ENCODED_4_CHARS 0xf0
#define UTF_8_ENCODED_3_CHARS 0xe0
#define UTF_8_ENCODED_2_CHARS 0xc0

#define NULL_REPLACEMENT "_hidden_null_"

/*
 * Translates UTF-8 characters (Unicode Translation Format - 8 Bit) into
 * Unicode and then runs the translation table.
 */
char *clean_utf_8(char *s, void *opts)
{
    char *output, *input_walk, *output_walk, *replace_walk;
    int new_value, expected_chars;

    struct translation_table *table = NULL;
    struct clean_string_options *options = NULL;

    int characters_eaten;

    if (s == NULL) {
        return NULL;
    }

    if (opts == NULL) {
        fprintf(stderr, "this shouldn't happen\n");
        exit(EXIT_FAILURE);
    }

    options = (struct clean_string_options *) opts;
    table = options->translation_table;

    output = malloc((strlen(s) * table->max_data_length) + 1);
    if (output == NULL) {
        fprintf(stderr, "out of memory: %s\n", strerror(errno));
        return NULL;
    }

    input_walk = s;
    output_walk = output;

    while (*input_walk != '\0') {
        new_value = 0;
        expected_chars = 0;
        characters_eaten = 0;

        /*
         * Needs to be done in descending orders due to the fact that
         * the 2 char mask will match on the 4 char mask, but not
         * vice versa.
         */
        if ((*input_walk & UTF_8_ENCODED_4_CHARS) == UTF_8_ENCODED_4_CHARS) {

            /*
             * 11110aaa 10bbbbbb 10cccccc 10dddddd
             */

            new_value = *input_walk & 0x07;
            expected_chars = 3;
            characters_eaten = 4;
        } else if ((*input_walk & UTF_8_ENCODED_3_CHARS) == UTF_8_ENCODED_3_CHARS) {

            /*
             * 1110aaaa 10bbbbbb 10cccccc
             */

            new_value = *input_walk & 0x0f;
            expected_chars = 2;
            characters_eaten = 3;
        } else if ((*input_walk & UTF_8_ENCODED_2_CHARS) == UTF_8_ENCODED_2_CHARS) {

            /*
             * 110aaaaa 10bbbbbb
             */

            new_value = *input_walk & 0x1f;
            expected_chars = 1;
            characters_eaten = 2;
        } else if ((*input_walk & UTF_8_ENCODED) == UTF_8_ENCODED) {
            fprintf(stderr, "unsupported unicode length\n");
            exit(EXIT_FAILURE);
        } else {
            new_value = *input_walk;
            expected_chars = 0;
            characters_eaten = 1;
        }

        while (expected_chars > 0) {
            new_value <<= 6;

            input_walk++;

            if (*input_walk == '\0') {
                new_value = -1;
                break;
            }

            if ((*input_walk & UTF_8_ENCODED) == 0) {
                new_value = -1;
                break;
            }

            new_value += *input_walk & 0x3f;

            expected_chars--;
        }
        input_walk++;

        if (new_value == -1) {
            continue;
        }

        replace_walk = table_get(table, new_value);

        //
        // Never allow a NULL encoded into 2+ byte UTF-8 to persist.
        //

        if (replace_walk == NULL && new_value == 0) {
            replace_walk = NULL_REPLACEMENT;
        }

        if (replace_walk == NULL) {
            replace_walk = table->default_translation;
        }

        if (replace_walk == NULL) {

            /*
             * Null translation == leave it alone
             */
            input_walk -= characters_eaten;

            while (characters_eaten > 0) {
                *output_walk++ = *input_walk++;
                characters_eaten--;
            }

            continue;
        }

        while (*replace_walk != '\0') {
            *output_walk++ = *replace_walk++;
        }
    }

    *output_walk = '\0';

    return output;
}


/*
 * Trims a file down to specified length.
 */
char *clean_max_length(char *s, void *opts)
{
    char *output, *input_walk, *output_walk;
    size_t max_length;
    size_t s_length;
    size_t ext_length;

    if (s == NULL) {
        return NULL;
    }

    max_length = 256;
    if (opts != NULL) {
        max_length = ((struct clean_string_options *) opts)->max_length;
    }

    s_length = strlen(s);

    output = malloc(max_length + 1);
    if (output == NULL) {
        fprintf(stderr, "out of memory: %s\n", strerror(errno));
        return NULL;
    }

    snprintf(output, max_length + 1, "%s", s);

    if (s_length <= max_length) {
        return output;
    }

    input_walk = strrchr(s, '.');

    if (input_walk == NULL) {
        return output;
    }

    ext_length = strlen(input_walk);

    output_walk = output;
    output_walk += max_length - ext_length;

    while (*(output_walk - 1) == '.' && output_walk > output) {
        output_walk--;
    }

    snprintf(output_walk, ext_length + 1, "%s", input_walk);

    return output;
}


/*
 * Converts all characters to lowercase.
 */
char *clean_lower(char *s, void *opts)
{
    char *output, *input_walk, *output_walk;

    if (s == NULL) {
        return NULL;
    }

    output = malloc(strlen(s) + 1);
    if (output == NULL) {
        fprintf(stderr, "out of memory: %s\n", strerror(errno));
        return NULL;
    }

    input_walk = s;
    output_walk = output;

    while (*input_walk != '\0') {
        if (isupper(*input_walk)) {
            *output_walk++ = tolower(*input_walk++);
        } else {
            *output_walk++ = *input_walk++;
        }
    }

    *output_walk = '\0';

    return output;
}

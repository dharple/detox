/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "clean_string.h"
#include "table.h"

#define ISO8859_1_UPPER_BIT 0x80

/*
 * Translates ISO 8859-1 characters (Latin-1) into lower ASCII characters.
 */
char *clean_iso8859_1(char *filename, table_t *table)
{
    char *output, *input_walk, *output_walk, *replace_walk;
    int new_value;

    if (filename == NULL) {
        return NULL;
    }

    if (table == NULL) {
        fprintf(stderr, "internal error\n");
        exit(EXIT_FAILURE);
    }

    output = malloc((strlen(filename) * table->max_data_length) + 1);
    if (output == NULL) {
        fprintf(stderr, "out of memory: %s\n", strerror(errno));
        return NULL;
    }

    input_walk = filename;
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
char *clean_safe(char *filename, table_t *table)
{
    char *output, *input_walk, *output_walk, *replace_walk;

    if (filename == NULL) {
        return NULL;
    }

    if (table == NULL) {
        fprintf(stderr, "internal error\n");
        exit(EXIT_FAILURE);
    }

    output = malloc((strlen(filename) * table->max_data_length) + 1);
    if (output == NULL) {
        fprintf(stderr, "out of memory: %s\n", strerror(errno));
        return NULL;
    }

    input_walk = filename;
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
char *clean_uncgi(char *filename)
{
    char *output, *input_walk, *output_walk;
    char conv[3];

    if (filename == NULL) {
        return NULL;
    }

    output = malloc(strlen(filename) + 1);
    if (output == NULL) {
        fprintf(stderr, "out of memory: %s\n", strerror(errno));
        return NULL;
    }

    input_walk = filename;
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


/**
 * Reduces any series of underscores or dashes to a single character.  The dash
 * takes precedence.
 *
 * If remove_trailing is set, then periods are added to the set of characters
 * to work on.  The period then takes precedence, followed by the dash.
 *
 * If a hash character, underscore, or dash are present at the start of the
 * filename, they will be removed.
 */
char *clean_wipeup(char *filename, int remove_trailing)
{
    char *output, *input_walk, *output_walk;
    char *search, *seek, *current;

    if (filename == NULL) {
        return NULL;
    }

    /* remove any -, _, or # at beginning of string */
    while (*filename == '-' || *filename == '_' || *filename == '#') {
        filename++;
    }

    output = malloc(strlen(filename) + 1);
    if (output == NULL) {
        fprintf(stderr, "out of memory: %s\n", strerror(errno));
        return NULL;
    }

    search = strdup(remove_trailing ? ".-_" : "-_");

    input_walk = filename;
    output_walk = output;
    current = NULL;

    while (*input_walk != '\0') {
        seek = strchr(search, *input_walk);
        if (seek != NULL) {
            if (current == NULL || seek < current) {
                current = seek;
            }

            input_walk++;
            continue;
        }

        if (current != NULL) {
            *output_walk++ = current[0];
            current = NULL;
        }


        *output_walk++ = *input_walk++;
    }

    if (current != NULL) {
        *output_walk++ = current[0];
    }

    *output_walk = '\0';

    free(search);

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
char *clean_utf_8(char *filename, table_t *table)
{
    char *output, *input_walk, *output_walk, *replace_walk;
    int new_value, expected_chars;

    int characters_eaten;

    if (filename == NULL) {
        return NULL;
    }

    if (table == NULL) {
        fprintf(stderr, "internal error\n");
        exit(EXIT_FAILURE);
    }

    output = malloc((strlen(filename) * table->max_data_length) + 1);
    if (output == NULL) {
        fprintf(stderr, "out of memory: %s\n", strerror(errno));
        return NULL;
    }

    input_walk = filename;
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
char *clean_max_length(char *filename, size_t max_length)
{
    char *extension;
    char *input_walk;
    char *output;
    size_t body_length;
    size_t extension_length;

    if (filename == NULL) {
        return NULL;
    }

    if (max_length <= 0) {
        max_length = 256;
    }

    // check to see if the file is smaller than the max length
    if (strlen(filename) <= max_length) {
        return strdup(filename);
    }

    output = malloc(max_length + 1);
    if (output == NULL) {
        fprintf(stderr, "out of memory: %s\n", strerror(errno));
        return NULL;
    }
    snprintf(output, max_length + 1, "%s", filename);

    // check to see if the file has no extension
    extension = input_walk = strrchr(filename, '.');
    if (input_walk == NULL) {
        return output;
    }

    if (strlen(extension) == 1) {
        return output;
    }

    // look back 5 characters for a second extension
    while (--input_walk > filename) {
        if (extension - input_walk > 5) {
            break;
        }

        if (*input_walk == '.') {
            extension = input_walk;
            break;
        }
    }

    extension_length = strlen(extension);
    if (max_length <= extension_length) {
        fprintf(stderr, "warning: max_length %d is less than required file length for '%s'.  giving up.\n", (int) max_length, filename);
        free(output);
        return strdup(filename);
    }

    body_length = max_length - extension_length;

    snprintf(output + body_length, extension_length + 1, "%s", extension);

    return output;
}

/*
 * Converts all characters to lowercase.
 */
char *clean_lower(char *filename)
{
    char *output, *input_walk, *output_walk;

    if (filename == NULL) {
        return NULL;
    }

    output = malloc(strlen(filename) + 1);
    if (output == NULL) {
        fprintf(stderr, "out of memory: %s\n", strerror(errno));
        return NULL;
    }

    input_walk = filename;
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

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

#include "clean_utf_8.h"
#include "table.h"
#include "wrapped.h"

#define UNICODE_MAX_VALUE 0x10FFFF

static char *null_replacement = "_hidden_null_";
static char *invalid_replacement = "_";

#define UTF_8_ENCODED_MASK  0xC0
#define UTF_8_ENCODED_START 0xC0
#define UTF_8_ENCODED_CONT  0x80

#define UTF_8_ENCODED_6_BYTES_MASK 0xFE
#define UTF_8_ENCODED_6_BYTES      0xFC

#define UTF_8_ENCODED_5_BYTES_MASK 0xFC
#define UTF_8_ENCODED_5_BYTES      0xF8

#define UTF_8_ENCODED_4_BYTES_MASK 0xF8
#define UTF_8_ENCODED_4_BYTES      0xF0

#define UTF_8_ENCODED_3_BYTES_MASK 0xF0
#define UTF_8_ENCODED_3_BYTES      0xE0

#define UTF_8_ENCODED_2_BYTES_MASK 0xE0
#define UTF_8_ENCODED_2_BYTES      0xC0

#define UPPER_BIT 0x80

#define check_width(chr, size) if ((chr & UTF_8_ENCODED_ ## size ## _BYTES_MASK) == UTF_8_ENCODED_ ## size ## _BYTES) { return size; }
#define is_upper_bit_set(chr) ((chr & UPPER_BIT) == UPPER_BIT)
#define unpack_cont(chr) ((unsigned char) chr & ~UTF_8_ENCODED_MASK)
#define unpack_start(chr, size) ((unsigned char) chr & ~UTF_8_ENCODED_ ## size ## _BYTES_MASK)

/**
 * Translates UTF-8 characters (Unicode Translation Format - 8 Bit) into
 * Unicode and then runs the transliteration table.
 *
 * @param filename The filename to clean.
 * @param table_t  The transliteration table to use.
 *
 * @return The updated filename or NULL if an error occurred.
 */
char *clean_utf_8(char *filename, table_t *table)
{
    char *output, *input_walk, *output_walk, *replace_walk;
    unsigned int new_value;
    int expected_chars;
    int characters_eaten;
    int utf_8_width;
    int failed;

    if (filename == NULL) {
        return NULL;
    }

    if (table == NULL) {
        fprintf(stderr, "internal error\n");
        exit(EXIT_FAILURE);
    }

    output = wrapped_malloc((strlen(filename) * table->max_data_length) + 1);

    input_walk = filename;
    output_walk = output;

    while (*input_walk != '\0') {
        utf_8_width = get_utf_8_width(*input_walk);

        switch (utf_8_width) {
            case 1: // 0aaaaaaa
                new_value = (unsigned char) input_walk[0];
                break;

            case 2: // 110aaaaa 10bbbbbb
                new_value = unpack_start(*input_walk, 2);
                break;

            case 3: // 1110aaaa 10bbbbbb 10cccccc
                new_value = unpack_start(*input_walk, 3);
                break;

            case 4: // 11110aaa 10bbbbbb 10cccccc 10dddddd
                new_value = unpack_start(*input_walk, 4);
                break;

            case 5: // 111110aa 10bbbbbb 10cccccc 10dddddd 10eeeeee
                new_value = unpack_start(*input_walk, 5);
                break;

            case 6: // 1111110a 10bbbbbb 10cccccc 10dddddd 10ffffff
                new_value = unpack_start(*input_walk, 6);
                break;

            default: // -1
                fprintf(stderr, "detox: warning: invalid encoding\n");
                *output_walk++ = invalid_replacement[0];
                input_walk++;
                continue;
        }

        expected_chars = utf_8_width - 1;
        characters_eaten = utf_8_width;

        failed = 0;

        while (expected_chars > 0) {
            new_value <<= 6;

            input_walk++;

            if (*input_walk == '\0') {
                fprintf(stderr, "detox: warning: UTF-8 sequence ended unexpectedly (null)\n");
                failed = 1;
                break;
            }

            if (!is_utf_8_cont(*input_walk)) {
                fprintf(stderr, "detox: warning: UTF-8 sequence ended unexpectedly (missing con't)\n");
                failed = 1;
                break;
            }

            new_value += unpack_cont(*input_walk);

            expected_chars--;
        }

        if (failed) {
            *output_walk++ = invalid_replacement[0];
            continue;
        }

#ifdef DEBUG
        if (utf_8_width > 1) {
            fprintf(stderr, "detox: debug: found character 0x%04x, width: %d\n", new_value, utf_8_width);
        }
#endif

        input_walk++;

        replace_walk = table_get(table, new_value);

        //
        // Never allow a NULL encoded into 2+ byte UTF-8 to persist.
        //

        if (replace_walk == NULL && new_value == 0) {
            fprintf(stderr, "detox: warning: UTF-8 encoded NULL encountered\n");
            replace_walk = null_replacement;
        }

        //
        // If we're off the top, take off, nuke it from orbit.  It's the only
        // way to be sure.
        //

        if (replace_walk == NULL && new_value > UNICODE_MAX_VALUE) {
            fprintf(stderr, "detox: warning: unicode character 0x%08X is invalid\n", new_value);
            replace_walk = invalid_replacement;
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

/**
 * Returns the width, in bytes, of a UTF-8 character, based on the start
 * character.
 *
 * @param c The character to examine.
 *
 * @return An integer between 1 and 6.  If the character is invalid, -1 will be
 *         returned.
 */
int get_utf_8_width(char c)
{
    if (is_utf_8_start(c)) {
        check_width(c, 2);
        check_width(c, 3);
        check_width(c, 4);
        check_width(c, 5);
        check_width(c, 6);
    }

    if (is_upper_bit_set(c)) {
        return -1;
    }

    return 1;
}

/**
 * Returns true if the character is part of a UTF-8 sequence, but not the first
 * byte.
 *
 * @param c The character to examine.
 *
 * @return bool
 */
int is_utf_8_cont(char c)
{
    return ((c & UTF_8_ENCODED_MASK) == UTF_8_ENCODED_CONT);
}

/**
 * Returns true if the character is the start of a UTF-8 sequence.
 *
 * @param c The character to examine.
 *
 * @return bool
 */
int is_utf_8_start(char c)
{
    return ((c & UTF_8_ENCODED_MASK) == UTF_8_ENCODED_START);
}

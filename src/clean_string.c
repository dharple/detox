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

#include "clean_string.h"
#include "wrapped.h"
#include "table.h"

#define ISO8859_1_UPPER_BIT 0x80

/**
 * Transliterates ISO 8859-1 characters (Latin-1) into lower ASCII characters.
 *
 * @param filename The filename to clean.
 * @param table_t  The transliteration table to use.
 *
 * @return The updated filename or NULL if an error occurred.
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

    output = wrapped_malloc((strlen(filename) * table->max_data_length) + 1);

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

/**
 * Replaces difficult-to-work-with characters with underscores and dashes.
 *
 * @param filename The filename to clean.
 * @param table_t  The replacement table to use.
 *
 * @return The updated filename or NULL if an error occurred.
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

    output = wrapped_malloc((strlen(filename) * table->max_data_length) + 1);

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

/**
 * Cleans up any CGI encoded characters, in the form "%" followed by 2 hex
 * digits.
 *
 * @param filename The filename to clean.
 *
 * @return The updated filename or NULL if an error occurred.
 */
char *clean_uncgi(char *filename)
{
    char *output, *input_walk, *output_walk;
    char conv[3];

    if (filename == NULL) {
        return NULL;
    }

    output = wrapped_malloc(strlen(filename) + 1);

    input_walk = filename;
    output_walk = output;

    while (*input_walk != '\0') {
        if (input_walk[0] == '%' && isxdigit((int) input_walk[1]) && isxdigit((int) input_walk[2])) {
            conv[0] = input_walk[1];
            conv[1] = input_walk[2];
            conv[2] = 0;
            *output_walk++ = (char) strtol(conv, NULL, 16);
            input_walk += 3;
        } else if (input_walk[0] == '+') {
            *output_walk++ = ' ';
            input_walk++;
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
 *
 * @param filename        The filename to clean.
 * @param remove_trailing Whether or not to include periods in the set of
 *                        characters to operate on.
 *
 * @return The updated filename or NULL if an error occurred.
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

    output = wrapped_malloc(strlen(filename) + 1);

    search = wrapped_strdup(remove_trailing ? ".-_" : "-_");

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
        return wrapped_strdup(filename);
    }

    output = wrapped_malloc(max_length + 1);

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
        return wrapped_strdup(filename);
    }

    body_length = max_length - extension_length;

    snprintf(output + body_length, extension_length + 1, "%s", extension);

    return output;
}

/**
 * Converts all characters to lowercase.
 *
 * @param filename        The filename to clean.
 *
 * @return The updated filename or NULL if an error occurred.
 */
char *clean_lower(char *filename)
{
    char *output, *input_walk, *output_walk;

    if (filename == NULL) {
        return NULL;
    }

    output = wrapped_malloc(strlen(filename) + 1);

    input_walk = filename;
    output_walk = output;

    while (*input_walk != '\0') {
        if (isupper((int) *input_walk)) {
            *output_walk++ = tolower((int) *input_walk++);
        } else {
            *output_walk++ = *input_walk++;
        }
    }

    *output_walk = '\0';

    return output;
}

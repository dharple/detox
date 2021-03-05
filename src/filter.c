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
#include <errno.h>
#include <string.h>

#include "detox_struct.h"

#include "builtin_table.h"
#include "clean_string.h"
#include "clean_utf_8.h"
#include "parse_table.h"
#include "wrapped.h"

/**
 * Attempts to find a translation table associated with a filter.
 *
 * @param check_filename The filename to look for.
 *
 * @return
 */
table_t *filter_find_table(const char *check_filename)
{
    table_t *table = NULL;
    int err;
    char *check_config_file;

    check_config_file = alloca(MAX_PATH_LEN);
    if (check_config_file == NULL) {
        fprintf(stderr, "out of memory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

#ifdef DATADIR
    err = snprintf(check_config_file, MAX_PATH_LEN, "%s/detox/%s", DATADIR, check_filename);
    if (err < MAX_PATH_LEN) {
        table = parse_table(check_config_file);
        if (table != NULL) {
            return table;
        }
    }
#endif

    err = snprintf(check_config_file, MAX_PATH_LEN, "/usr/share/detox/%s", check_filename);
    if (err < MAX_PATH_LEN) {
        table = parse_table(check_config_file);
        if (table != NULL) {
            return table;
        }
    }

    err = snprintf(check_config_file, MAX_PATH_LEN, "/usr/local/share/detox/%s", check_filename);
    if (err < MAX_PATH_LEN) {
        table = parse_table(check_config_file);
    }

    return table;
}


filter_t *filter_init(int cleaner)
{
    filter_t *ret;

    ret = wrapped_malloc(sizeof(filter_t));

    memset(ret, 0, sizeof(filter_t));

    ret->cleaner = cleaner;

    return ret;
}

/**
 * Uses a builtin table for a filter.
 *
 * @param filter The filter to load a builtin for.
 */
table_t *filter_load_builtin(filter_t *filter)
{
    if (filter->cleaner == FILTER_ISO8859_1) {
        return load_builtin_iso8859_1_table();
    } else if (filter->cleaner == FILTER_UTF_8) {
        return load_builtin_unicode_table();
    } else if (filter->cleaner == FILTER_SAFE) {
        return load_builtin_safe_table();
    }

    return NULL;
}

/**
 * Uses a builtin table for a filter.
 *
 * Valid values:
 * - builtin:cp1252
 * - builtin:iso8859_1
 * - builtin:safe
 * - builtin:unicode
 *
 * @param filename Builtin filename to use
 */
table_t *filter_load_builtin_by_filename(const char *filename)
{
    if (strcmp(filename, "iso8859_1") == 0) {
        return load_builtin_iso8859_1_table();
    } else if (strcmp(filename, "unicode") == 0) {
        return load_builtin_unicode_table();
    } else if (strcmp(filename, "safe") == 0) {
        return load_builtin_safe_table();
    } else if (strcmp(filename, "cp1252") == 0) {
        return load_builtin_cp1252_table();
    }

    return NULL;
}

/**
 * Loads the translation table associated with a filter.
 *
 * @param filter The filter to check.
 */
table_t *filter_load_table(filter_t *filter)
{
    table_t *table = NULL;
    char *check_filename = NULL;
    int do_search = 1;

    if (filter->cleaner == FILTER_ISO8859_1) {
        check_filename = "iso8859_1.tbl";
    } else if (filter->cleaner == FILTER_UTF_8) {
        check_filename = "unicode.tbl";
    } else if (filter->cleaner == FILTER_SAFE) {
        check_filename = "safe.tbl";
    } else {
        return NULL;
    }

    if (filter->builtin != NULL) {
        table = filter_load_builtin_by_filename(filter->builtin);
        if (table == NULL) {
            fprintf(stderr, "detox: unable to locate builtin table \"%s\"\n", filter->builtin);
            exit(EXIT_FAILURE);
        }
        return table;
    }

    if (filter->filename != NULL) {
        check_filename = filter->filename;
        do_search = 0;
    }

    if (do_search) {
        table = filter_find_table(check_filename);
        if (table != NULL) {
            return table;
        }

        // load builtin translation tables
        table = filter_load_builtin(filter);

        if (table == NULL) {
            fprintf(stderr, "detox: unable to locate translation table or fall back\n");
            exit(EXIT_FAILURE);
        }
    } else {
        table = parse_table(check_filename);

        if (table == NULL) {
            fprintf(stderr, "detox: unable to parse file: \"%s\"\n", check_filename);
            exit(EXIT_FAILURE);
        }
    }

    return table;
}

/**
 * Runs a filter.
 *
 * @param filter   The filter to run.
 * @param filename The filename to run through the filter.
 *
 * @return The filtered filename, or NULL if a problem was encountered.
 */
char *filter_run(filter_t *filter, char *filename)
{
    char *ret;

    if (filter == NULL) {
        fprintf(stderr, "internal error\n");
        exit(EXIT_FAILURE);
    }

    if (filename == NULL) {
        return NULL;
    }

    switch (filter->cleaner) {
        case FILTER_ISO8859_1:
            ret = clean_iso8859_1(filename, filter->table);
            break;

        case FILTER_LOWER:
            ret = clean_lower(filename);
            break;

        case FILTER_MAX_LENGTH:
            ret = clean_max_length(filename, filter->max_length);
            break;

        case FILTER_SAFE:
            ret = clean_safe(filename, filter->table);
            break;

        case FILTER_UNCGI:
            ret = clean_uncgi(filename);
            break;

        case FILTER_UTF_8:
            ret = clean_utf_8(filename, filter->table);
            break;

        case FILTER_WIPEUP:
            ret = clean_wipeup(filename, filter->remove_trailing);
            break;

        default:
            fprintf(stderr, "detox: unknown filter %d\n", filter->cleaner);
            exit(EXIT_FAILURE);
    }

    return ret;
}

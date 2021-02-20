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

#include "detox.h"
#include "builtin_table.h"
#include "parse_table.h"
#include "clean_string.h"

/**
 * Chooses which sequence to use.
 *
 * @param sequences     Sequences from the config file.
 * @param sequence_name The sequence name from the command line, if any.
 *
 * @return The chosen sequence.
 */
struct detox_sequence_entry *sequence_choose_default(struct detox_sequence_list *sequences, char *sequence_name)
{
    struct detox_sequence_entry *which = NULL;
    struct detox_sequence_list *work = sequences;

    while (work != NULL) {
        if (strcmp(work->name, (sequence_name == NULL) ? "default" : sequence_name) == 0) {
            which = work->head;
            break;
        }

        work = work->next;
    }

    /*
     * If no sequence was found, and the user didn't specify a sequence
     * to use, just use the first sequence.
     */

    if (which == NULL && sequence_name == NULL) {
        if (sequences != NULL) {
            which = sequences->head;
        }
    }

    return which;
}

/**
 * Attempts to find a translation table associated with a sequence.
 *
 * @param check_filename The filename to look for.
 *
 * @return
 */
struct translation_table *sequence_find_table(const char *check_filename)
{
    struct translation_table *table = NULL;
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

/**
 * Uses a builtin table for a sequence.
 *
 * @param sequence The sequence to load a builtin for.
 */
struct translation_table *sequence_load_builtin(struct detox_sequence_entry *sequence)
{
    if (sequence->cleaner == &clean_iso8859_1) {
        return load_builtin_iso8859_1_table();
    } else if (sequence->cleaner == &clean_utf_8) {
        return load_builtin_unicode_table();
    } else if (sequence->cleaner == &clean_safe) {
        return load_builtin_safe_table();
    }

    return NULL;
}

/**
 * Checks the file associated with a sequence.
 *
 * @param sequence The sequence to check.
 */
void sequence_check_file(struct detox_sequence_entry *sequence)
{
    struct clean_string_options *opts = NULL;
    struct translation_table *table = NULL;
    char *check_filename = NULL;
    int do_search = 1;

    if (sequence->cleaner == &clean_iso8859_1) {
        check_filename = "iso8859_1.tbl";
    } else if (sequence->cleaner == &clean_utf_8) {
        check_filename = "unicode.tbl";
    } else if (sequence->cleaner == &clean_safe) {
        check_filename = "safe.tbl";
    } else {
        return;
    }

    if (sequence->options != NULL) {
        opts = sequence->options;
        if (opts->filename != NULL) {
            check_filename = opts->filename;
            do_search = 0;
        }
    }

    if (do_search) {
        table = sequence_find_table(check_filename);

        // load builtin translation tables
        if (table == NULL) {
            table = sequence_load_builtin(sequence);
        }

        if (table == NULL) {
            fprintf(stderr, "detox: unable to locate translation table or fall back\n");
            exit(EXIT_FAILURE);
        }

        // Allocate an options struct
        opts = malloc(sizeof(struct clean_string_options));
        if (opts == NULL) {
            fprintf(stderr, "out of memory: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        memset(opts, 0, sizeof(struct clean_string_options));

        opts->translation_table = table;
        sequence->options = opts;
    } else {
        table = parse_table(check_filename);
        if (table == NULL) {
            fprintf(stderr, "detox: unable to parse file: %s\n", check_filename);
            exit(EXIT_FAILURE);
        }

        opts = sequence->options;
        opts->translation_table = table;
    }
}

/**
 * Reviews a sequence to confirm that it's valid.
 *
 * @param struct detox_sequence_entry *sequence
 *
 * @return void
 */
void sequence_review(struct detox_sequence_entry *sequence)
{
    struct detox_sequence_entry *work = sequence;

    while (work != NULL) {
        sequence_check_file(work);
        work = work->next;
    }
}

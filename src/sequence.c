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
 * @param struct detox_sequence_list *sequences     Sequences from the config file.
 * @param char                       *sequence_name The sequence name from the command line, if any.
 *
 * @return struct detox_sequence_entry
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
 * Reviews a sequence to confirm that it's valid.
 *
 * @param struct detox_sequence_entry *sequence
 *
 * @return void
 */
void sequence_review(struct detox_sequence_entry *sequence)
{
    struct detox_sequence_entry *work = sequence;
    char *check_config_file = NULL;
    int err;

    while (work != NULL) {
        char *check_filename = NULL;
        int do_search = 0;

        struct translation_table *table = NULL;
        struct clean_string_options *opts;

        if (work->cleaner == &clean_iso8859_1) {
            if (work->options != NULL) {
                opts = work->options;
                if (opts->filename != NULL) {
                    check_filename = opts->filename;
                }
            }

            if (!check_filename) {
                check_filename = "iso8859_1.tbl";
                do_search = 1;
            }
        } else if (work->cleaner == &clean_utf_8) {
            if (work->options != NULL) {
                opts = work->options;
                if (opts->filename != NULL) {
                    check_filename = opts->filename;
                }
            }

            if (!check_filename) {
                check_filename = "unicode.tbl";
                do_search = 1;
            }
        } else if (work->cleaner == &clean_safe) {
            if (work->options != NULL) {
                opts = work->options;
                if (opts->filename != NULL) {
                    check_filename = opts->filename;
                }
            }

            if (!check_filename) {
                check_filename = "safe.tbl";
                do_search = 1;
            }
        }

        if (check_filename || do_search) {

            table = NULL;

            if (do_search) {
                check_config_file = malloc(MAX_PATH_LEN);
                if (check_config_file == NULL) {
                    fprintf(stderr, "out of memory: %s\n", strerror(errno));
                    exit(EXIT_FAILURE);
                }

#ifdef DATADIR
                err = snprintf(check_config_file, MAX_PATH_LEN, "%s/detox/%s", DATADIR, check_filename);
                if (err < MAX_PATH_LEN) {
                    table = parse_table(check_config_file);
                }
#endif

                if (table == NULL) {
                    err = snprintf(check_config_file, MAX_PATH_LEN, "/usr/share/detox/%s", check_filename);
                    if (err < MAX_PATH_LEN) {
                        table = parse_table(check_config_file);
                    }
                }

                if (table == NULL) {
                    err = snprintf(check_config_file, MAX_PATH_LEN, "/usr/local/share/detox/%s", check_filename);
                    if (err < MAX_PATH_LEN) {
                        table = parse_table(check_config_file);
                    }
                }

                // load builtin translation tables
                if (table == NULL) {
                    if (work->cleaner == &clean_iso8859_1) {
                        table = load_builtin_iso8859_1_table();
                    } else if (work->cleaner == &clean_utf_8) {
                        table = load_builtin_unicode_table();
                    } else if (work->cleaner == &clean_safe) {
                        table = load_builtin_safe_table();
                    }
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
                work->options = opts;

                free(check_config_file);
            } else {
                table = parse_table(check_filename);
                if (table == NULL) {
                    fprintf(stderr, "detox: unable to parse file: %s\n", check_filename);
                    exit(EXIT_FAILURE);
                }

                opts = work->options;
                opts->translation_table = table;
            }
        }

        work = work->next;
    }
}

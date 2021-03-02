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

#include "builtin_table.h"
#include "clean_string.h"
#include "detox_struct.h"
#include "filter.h"
#include "parse_table.h"
#include "sequence.h"

/**
 * Chooses which sequence to use.
 *
 * @param sequences Sequences from the config file.
 * @param name      The sequence name from the command line, if any.
 *
 * @return The chosen sequence.
 */
sequence_t *sequence_choose_default(sequence_t *sequences, const char *name)
{
    sequence_t *which = NULL;
    sequence_t *work = sequences;

    while (work != NULL) {
        if (strcmp(work->name, (name == NULL) ? "default" : name) == 0) {
            which = work;
            break;
        }

        work = work->next;
    }

    /*
     * If no sequence was found, and the user didn't specify a sequence
     * to use, just use the first sequence.
     */

    if (which == NULL && name == NULL) {
        if (sequences != NULL) {
            which = sequences;
        }
    }

    return which;
}

sequence_t *sequence_init(const char *name)
{
    sequence_t *ret;

    ret = malloc(sizeof(sequence_t));
    if (ret == NULL) {
        fprintf(stderr, "detox: out of memory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    memset(ret, 0, sizeof(sequence_t));

    ret->name = strdup(name);

    return ret;
}

/**
 * Reviews a sequence to confirm that it's valid.
 *
 * @param sequence_t *sequence
 *
 * @return void
 */
void sequence_review(sequence_t *sequence)
{
    filter_t *filter = sequence->filters;
    table_t *table = NULL;

    while (filter != NULL) {
        table = filter_load_table(filter);

        if (table != NULL) {
            filter->table = table;
        }
        filter = filter->next;
    }
}

/**
 *
 */
char *sequence_run_filters(sequence_t *sequence, char *in)
{
    filter_t *filter;
    char *work, *hold;

    if (sequence == NULL) {
        fprintf(stderr, "internal error\n");
        exit(EXIT_FAILURE);
    }

    if (in == NULL) {
        return NULL;
    }

    filter = sequence->filters;
    work = strdup(in);

    while (filter != NULL) {
        switch (filter->cleaner) {
            case FILTER_ISO8859_1:
                hold = clean_iso8859_1(work, filter->table);
                break;

            case FILTER_LOWER:
                hold = clean_lower(work);
                break;

            case FILTER_MAX_LENGTH:
                hold = clean_max_length(work, filter->max_length);
                break;

            case FILTER_SAFE:
                hold = clean_safe(work, filter->table);
                break;

            case FILTER_UNCGI:
                hold = clean_uncgi(work);
                break;

            case FILTER_UTF_8:
                hold = clean_utf_8(work, filter->table);
                break;

            case FILTER_WIPEUP:
                hold = clean_wipeup(work, filter->remove_trailing);
                break;
        }

        if (work != NULL) {
            free(work);
        }

        if (hold == NULL) {
            return NULL;
        }

        work = hold;

        filter = filter->next;
    }

    return work;
}

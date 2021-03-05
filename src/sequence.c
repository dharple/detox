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

#include "detox_struct.h"

#include "filter.h"
#include "sequence.h"
#include "wrapped.h"

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

    ret = wrapped_malloc(sizeof(sequence_t));
    memset(ret, 0, sizeof(sequence_t));

    ret->name = wrapped_strdup(name);

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
 * Runs the filters associated with a sequence.
 *
 * @param sequence The sequence of filters to run.
 * @param filename The filename to run through the filters.
 *
 * @return The filtered filename, or NULL if a problem was encountered.
 */
char *sequence_run_filters(sequence_t *sequence, char *filename)
{
    filter_t *filter;
    char *hold;
    char *work;

    if (sequence == NULL) {
        fprintf(stderr, "internal error\n");
        exit(EXIT_FAILURE);
    }

    if (filename == NULL) {
        return NULL;
    }

    filter = sequence->filters;
    work = wrapped_strdup(filename);

    while (filter != NULL && work != NULL) {
        hold = filter_run(filter, work);
        free(work);
        work = hold;
        filter = filter->next;
    }

    return work;
}

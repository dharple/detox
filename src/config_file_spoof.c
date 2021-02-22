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
#include <errno.h>

#include "detox_struct.h"
#include "config_file_spoof.h"
#include "clean_string.h"

struct detox_parse_results *spoof_config_file(void)
{
    struct detox_parse_results *ret = NULL;
    struct detox_sequence_list *sequences = NULL;
    struct detox_sequence_entry *work = NULL;

    /*
     * Initialize return
     */

    ret = malloc(sizeof(struct detox_parse_results));
    if (ret == NULL) {
        fprintf(stderr, "out of memory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    memset(ret, 0, sizeof(struct detox_parse_results));

    /*
     * Head of sequence
     */

    sequences = malloc(sizeof(struct detox_sequence_list));
    if (sequences == NULL) {
        fprintf(stderr, "out of memory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    memset(sequences, 0, sizeof(struct detox_sequence_list));

    sequences->name = strdup("default");
    sequences->source_filename = strdup("built-in config file");

    /*
     * Step 1 - Safe
     */

    sequences->head = malloc(sizeof(struct detox_sequence_entry));
    if (sequences->head == NULL) {
        fprintf(stderr, "out of memory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    work = sequences->head;
    memset(work, 0, sizeof(struct detox_sequence_entry));

    work->cleaner = &clean_safe;

    // Allocate an options struct
    work->options = malloc(sizeof(struct clean_string_options));
    if (work->options == NULL) {
        fprintf(stderr, "out of memory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    memset(work->options, 0, sizeof(struct clean_string_options));
    work->options->builtin = strdup("safe");

    /*
     * Step 2 - Wipe Up
     */

    work->next = malloc(sizeof(struct detox_sequence_entry));
    if (work->next == NULL) {
        fprintf(stderr, "out of memory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    work = work->next;
    memset(work, 0, sizeof(struct detox_sequence_entry));

    work->cleaner = &clean_wipeup;

    /*
     *
     */

    ret->sequences = sequences;

    return ret;
}

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
    struct detox_sequence_filter *filter = NULL;

    /*
     * Initialize return
     */

    ret = new_detox_parse_results();

    /*
     * Head of sequence
     */

    sequences = new_detox_sequence_list();
    sequences->name = strdup("default");
    sequences->source_filename = strdup("built-in config file");

    /*
     * Step 1 - Safe
     */

    sequences->head = new_detox_sequence_filter();
    filter = sequences->head;
    filter->cleaner = &clean_safe;
    filter->options = new_clean_string_options();
    filter->options->builtin = strdup("safe");

    /*
     * Step 2 - Wipe Up
     */

    filter->next = new_detox_sequence_filter();
    filter = filter->next;
    filter->cleaner = &clean_wipeup;
    filter->options = new_clean_string_options();
    filter->options->remove_trailing = 1;

    /*
     *
     */

    ret->sequences = sequences;

    return ret;
}

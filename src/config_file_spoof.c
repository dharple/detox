/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <string.h>

#include "clean_string.h"
#include "config_file.h"
#include "config_file_spoof.h"
#include "detox_struct.h"
#include "filelist.h"

/**
 * Generates an ISO8859-1 style filter.
 *
 * @param builtin The name of the builtin table to use.
 *
 * @return
 */
static struct detox_sequence_filter *generate_iso8859_1_filter(char *builtin)
{
    struct detox_sequence_filter *filter;
    filter = new_detox_sequence_filter();
    filter->cleaner = &clean_iso8859_1;
    filter->options = new_clean_string_options();
    filter->options->builtin = strdup(builtin);
    return filter;
}

/**
 * Generates a lower filter.
 *
 * @return
 */
static struct detox_sequence_filter *generate_lower_filter(void)
{
    struct detox_sequence_filter *filter;
    filter = new_detox_sequence_filter();
    filter->cleaner = &clean_lower;
    return filter;
}

/**
 * Generates a safe filter.
 *
 * @return
 */
static struct detox_sequence_filter *generate_safe_filter(void)
{
    struct detox_sequence_filter *filter;
    filter = new_detox_sequence_filter();
    filter->cleaner = &clean_safe;
    filter->options = new_clean_string_options();
    filter->options->builtin = strdup("safe");
    return filter;
}

/**
 * Generates a new sequence.
 *
 * @param name The name to give the new sequence.
 *
 * @return
 */
static struct detox_sequence_list *generate_sequence(char *name)
{
    struct detox_sequence_list *sequence;
    sequence = new_detox_sequence_list();
    sequence->name = strdup(name);
    sequence->source_filename = strdup("built-in config file");
    return sequence;
}

/**
 * Generates an UNCGI filter.
 *
 * @return
 */
static struct detox_sequence_filter *generate_uncgi_filter(void)
{
    struct detox_sequence_filter *filter;
    filter = new_detox_sequence_filter();
    filter->cleaner = &clean_uncgi;
    return filter;
}

/**
 * Generates an UTF-8 style filter.
 *
 * @param builtin The name of the builtin table to use.
 *
 * @return
 */
static struct detox_sequence_filter *generate_utf_8_filter(char *builtin)
{
    struct detox_sequence_filter *filter;
    filter = new_detox_sequence_filter();
    filter->cleaner = &clean_utf_8;
    filter->options = new_clean_string_options();
    filter->options->builtin = strdup(builtin);
    return filter;
}

/**
 * Generates a wipeup filter.
 *
 * @param remove_trailing Whether or not to include periods in the reduction list.
 *
 * @return
 */
static struct detox_sequence_filter *generate_wipeup_filter(int remove_trailing)
{
    struct detox_sequence_filter *filter;
    filter = new_detox_sequence_filter();
    filter->cleaner = &clean_wipeup;
    filter->options = new_clean_string_options();
    filter->options->remove_trailing = remove_trailing;
    return filter;
}

/**
 * Out of nothing, a config file.
 *
 * @return
 */
config_file_t *spoof_config_file(void)
{
    config_file_t *ret = NULL;
    struct detox_sequence_list *sequence = NULL;
    struct detox_sequence_filter *filter = NULL;
    struct detox_sequence_filter *safe_wipeup_filter = NULL;

    // generate safe_wipeup_filter
    safe_wipeup_filter = generate_safe_filter();
    safe_wipeup_filter->next = generate_wipeup_filter(1);

    // build containing structure
    ret = config_file_init();

    // default sequence
    ret->sequences = sequence = generate_sequence("default");
    sequence->head = safe_wipeup_filter;

    // iso8859_1
    sequence = sequence->next = generate_sequence("iso8859_1");
    sequence->head = filter = generate_iso8859_1_filter("iso8859_1");
    filter->next = safe_wipeup_filter;

    // iso8859_1-legacy
    sequence = sequence->next = generate_sequence("iso8859_1-legacy");
    sequence->head = filter = generate_iso8859_1_filter("cp1252");
    filter = filter->next = generate_iso8859_1_filter("iso8859_1");
    filter->next = safe_wipeup_filter;

    // utf_8
    sequence = sequence->next = generate_sequence("utf_8");
    sequence->head = filter = generate_utf_8_filter("unicode");
    filter->next = safe_wipeup_filter;

    // utf_8-legacy
    sequence = sequence->next = generate_sequence("utf_8-legacy");
    sequence->head = filter = generate_utf_8_filter("cp1252");
    filter = filter->next = generate_utf_8_filter("unicode");
    filter->next = safe_wipeup_filter;

    // uncgi
    sequence = sequence->next = generate_sequence("uncgi");
    sequence->head = filter = generate_uncgi_filter();
    filter->next = safe_wipeup_filter;

    // lower
    sequence = sequence->next = generate_sequence("lower");
    sequence->head = filter = generate_safe_filter();
    filter = filter->next = generate_lower_filter();
    filter->next = generate_wipeup_filter(1);

    // iso8859_1-only
    sequence = sequence->next = generate_sequence("iso8859_1-only");
    sequence->head = generate_iso8859_1_filter("iso8859_1");

    // cp1252-only
    sequence = sequence->next = generate_sequence("cp1252-only");
    sequence->head = generate_iso8859_1_filter("cp1252");

    // utf_8-only
    sequence = sequence->next = generate_sequence("utf_8-only");
    sequence->head = generate_utf_8_filter("unicode");

    // uncgi-only
    sequence = sequence->next = generate_sequence("uncgi-only");
    sequence->head = generate_uncgi_filter();

    // lower-only
    sequence = sequence->next = generate_sequence("lower-only");
    sequence->head = generate_lower_filter();

    // files to ignore
    ret->files_to_ignore = filelist_init();
    filelist_put(ret->files_to_ignore, "{arch}");

    return ret;
}

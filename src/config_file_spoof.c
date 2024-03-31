/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "detox_struct.h"

#include "config_file.h"
#include "config_file_spoof.h"
#include "filelist.h"
#include "filter.h"
#include "sequence.h"
#include "wrapped.h"

/**
 * Generates a filter that uses a builtin table.
 *
 * @param builtin The name of the builtin table to use.
 *
 * @return
 */
static filter_t *generate_builtin_filter(int cleaner, char *builtin)
{
    filter_t *filter;
    filter = filter_init(cleaner);
    filter->builtin = wrapped_strdup(builtin);
    return filter;
}

/**
 * Generates a filter.
 *
 * @return
 */
static filter_t *generate_filter(int cleaner)
{
    filter_t *filter;
    filter = filter_init(cleaner);
    if (cleaner == FILTER_SAFE) {
        filter->builtin = wrapped_strdup("safe");
    }
    return filter;
}

/**
 * Generates a new sequence.
 *
 * @param name The name to give the new sequence.
 *
 * @return
 */
static sequence_t *generate_sequence(char *name)
{
    sequence_t *sequence;
    sequence = sequence_init(name);
    sequence->source_filename = wrapped_strdup("built-in config file");
    return sequence;
}

/**
 * Generates a wipeup filter.
 *
 * @param remove_trailing Whether or not to include periods in the reduction list.
 *
 * @return
 */
static filter_t *generate_wipeup_filter(int remove_trailing)
{
    filter_t *filter;
    filter = filter_init(FILTER_WIPEUP);
    filter->remove_trailing = remove_trailing;
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
    sequence_t *sequence = NULL;
    filter_t *filter = NULL;
    filter_t *safe_wipeup_filter = NULL;

    // generate safe_wipeup_filter
    safe_wipeup_filter = generate_filter(FILTER_SAFE);
    safe_wipeup_filter->next = generate_wipeup_filter(1);

    // build containing structure
    ret = config_file_init();

    // default sequence
    ret->sequences = sequence = generate_sequence("default");
    sequence->filters = safe_wipeup_filter;

    // iso8859_1
    sequence = sequence->next = generate_sequence("iso8859_1");
    sequence->filters = filter = generate_builtin_filter(FILTER_ISO8859_1, "iso8859_1");
    filter->next = safe_wipeup_filter;

    // iso8859_1-legacy
    sequence = sequence->next = generate_sequence("iso8859_1-legacy");
    sequence->filters = filter = generate_builtin_filter(FILTER_ISO8859_1, "cp1252");
    filter = filter->next = generate_builtin_filter(FILTER_ISO8859_1, "iso8859_1");
    filter->next = safe_wipeup_filter;

    // utf_8
    sequence = sequence->next = generate_sequence("utf_8");
    sequence->filters = filter = generate_builtin_filter(FILTER_UTF_8, "unicode");
    filter->next = safe_wipeup_filter;

    // uncgi
    sequence = sequence->next = generate_sequence("uncgi");
    sequence->filters = filter = generate_filter(FILTER_UNCGI);
    filter->next = safe_wipeup_filter;

    // lower
    sequence = sequence->next = generate_sequence("lower");
    sequence->filters = filter = generate_filter(FILTER_SAFE);
    filter = filter->next = generate_filter(FILTER_LOWER);
    filter->next = generate_wipeup_filter(1);

    // iso8859_1-only
    sequence = sequence->next = generate_sequence("iso8859_1-only");
    sequence->filters = generate_builtin_filter(FILTER_ISO8859_1, "iso8859_1");

    // cp1252-only
    sequence = sequence->next = generate_sequence("cp1252-only");
    sequence->filters = generate_builtin_filter(FILTER_ISO8859_1, "cp1252");

    // utf_8-only
    sequence = sequence->next = generate_sequence("utf_8-only");
    sequence->filters = generate_builtin_filter(FILTER_UTF_8, "unicode");

    // uncgi-only
    sequence = sequence->next = generate_sequence("uncgi-only");
    sequence->filters = generate_filter(FILTER_UNCGI);

    // lower-only
    sequence = sequence->next = generate_sequence("lower-only");
    sequence->filters = generate_filter(FILTER_LOWER);

    // files to ignore
    ret->files_to_ignore = filelist_init();
    filelist_put(ret->files_to_ignore, "{arch}");

    return ret;
}

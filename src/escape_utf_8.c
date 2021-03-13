/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "file.h"
#include "filter.h"
#include "parse_options.h"
#include "sequence.h"
#include "table.h"
#include "table_dump.h"
#include "wrapped.h"

int main(void)
{
    table_t *table;
    options_t *options;
    sequence_t *sequence;
    filter_t *filter;

    char *work;
    int i;

    work = wrapped_malloc(1024);

    table = table_init(0x110000);
    for (i = 0x01; i < 0x20; i++) {
        sprintf(work, "\\x%02X", i);
        table_put(table, i, work);
    }

    for (; i < 0x7F; i++) {
        sprintf(work, "%c", i);
        table_put(table, i, work);
    }

    for (; i < 0x80; i++) {
        sprintf(work, "\\x%02X", i);
        table_put(table, i, work);
    }

    for (; i < 0x10000; i++) {
        sprintf(work, "\\u%04X", i);
        table_put(table, i, work);
    }

    for (; i < 0x110000; i++) {
        sprintf(work, "\\U%08X", i);
        table_put(table, i, work);
    }

    options = options_init();

    options->sequence_to_use = sequence = sequence_init("utf_8-escape");
    sequence->source_filename = wrapped_strdup(__FILE__);

    sequence->filters = filter = filter_init(FILTER_UTF_8);
    filter->filename = wrapped_strdup(__FILE__);
    filter->table = table;

    parse_inline(NULL, NULL, options);

#ifdef DEBUG
    table_stats(table);
#endif
}

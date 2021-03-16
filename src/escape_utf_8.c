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

#define MAX_LENGTH 16

/**
 * Generated from c_escape.tbl
 */
static table_row_t builtin_c_escape_rows[36] = {
    { .key  = 0x0001, .data = "\\x01" },
    { .key  = 0x0002, .data = "\\x02" },
    { .key  = 0x0003, .data = "\\x03" },
    { .key  = 0x0004, .data = "\\x04" },
    { .key  = 0x0005, .data = "\\x05" },
    { .key  = 0x0006, .data = "\\x06" },
    { .key  = 0x0007, .data = "\\a" },
    { .key  = 0x0008, .data = "\\b" },
    { .key  = 0x0009, .data = "\\t" },
    { .key  = 0x000a, .data = "\\n" },
    { .key  = 0x000b, .data = "\\v" },
    { .key  = 0x000c, .data = "\\f" },
    { .key  = 0x000d, .data = "\\r" },
    { .key  = 0x000e, .data = "\\x0e" },
    { .key  = 0x000f, .data = "\\x0f" },
    { .key  = 0x0010, .data = "\\x10" },
    { .key  = 0x0011, .data = "\\x11" },
    { .key  = 0x0012, .data = "\\x12" },
    { .key  = 0x0013, .data = "\\x13" },
    { .key  = 0x0014, .data = "\\x14" },
    { .key  = 0x0015, .data = "\\x15" },
    { .key  = 0x0016, .data = "\\x16" },
    { .key  = 0x0017, .data = "\\x17" },
    { .key  = 0x0018, .data = "\\x18" },
    { .key  = 0x0019, .data = "\\x19" },
    { .key  = 0x001a, .data = "\\x1a" },
    { .key  = 0x001b, .data = "\\x1b" },
    { .key  = 0x001c, .data = "\\x1c" },
    { .key  = 0x001d, .data = "\\x1d" },
    { .key  = 0x001e, .data = "\\x1e" },
    { .key  = 0x001f, .data = "\\x1f" },
    { .key  = 0x0022, .data = "\\\"" },
    { .key  = 0x0027, .data = "\\'" },
    { .key  = 0x003f, .data = "\\?" },
    { .key  = 0x005c, .data = "\\\\" },
    { .key  = 0x007f, .data = "\\x7f" },
};

static table_t builtin_c_escape_table = {
    .length              = 36,
    .used                = 36,
    .max_data_length     = 4,
    .max_key             = 0x007f,
    .hits                = 0,
    .misses              = 0,
    .seeks               = 0,
    .overwrites          = 0,
    .use_hash            = 0,
    .builtin             = 1,
    .rows                = builtin_c_escape_rows,
    .default_translation = NULL,
};

/**
 * Generates a translation table that escapes everything not between 0x20 and
 * 0x7E (inclusive).
 *
 * For characters 0x01-0x1F and 0x7F, a \x escape sequence is used.  This could
 * be improved, e.g. use \n instead of \x0A and \t instead of \x09.
 *
 * For characters 0x80-
 */
table_t *generate_c_escape_table(void)
{
    table_t *table;
    char *work;
    int i;

    work = wrapped_malloc(MAX_LENGTH);

    table = table_resize(&builtin_c_escape_table, 0x110000, 1);

    for (i = 0x20; i < 0x7F; i++) {
        if (table_get(table, i) == NULL) {
            snprintf(work, MAX_LENGTH, "%c", i);
            table_put(table, i, work);
        }
    }

    for (i = 0x80; i < 0x10000; i++) {
        snprintf(work, MAX_LENGTH, "\\u%04X", i);
        table_put(table, i, work);
    }

    for (; i < 0x110000; i++) {
        snprintf(work, MAX_LENGTH, "\\U%08X", i);
        table_put(table, i, work);
    }

    free(work);

#ifdef DEBUG
    table_reset(table);
#endif

    return table;
}

/**
 * Reads from stdin and escapes everything so that:
 *
 * a) all Unicode is obvious, and
 * b) the output is safe to use in a C string.
 */
int main(void)
{
    table_t *table;
    options_t *options;
    sequence_t *sequence;
    filter_t *filter;

    table = generate_c_escape_table();

    options = options_init();

    options->sequence_to_use = sequence = sequence_init("utf_8-escape");
    sequence->source_filename = wrapped_strdup(__FILE__);

    sequence->filters = filter = filter_init(FILTER_UTF_8);
    filter->filename = wrapped_strdup(__FILE__);
    filter->table = table;

    parse_inline(NULL, NULL, options);

#ifdef DEBUG
    printf("\n\n");
    table_stats(table);
    table_free(table);
#endif
}

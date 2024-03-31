/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * Helper utility that generates conversion tables.
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <libgen.h>

#include "builtin_table.h"
#include "parse_table.h"
#include "table.h"
#include "wrapped.h"

/**
 * Escapes a string for this particular case.
 *
 * @param in
 *
 * @return
 */
static char *escape_string(char *in)
{
    char *ret, *work;

    ret = work = wrapped_malloc((strlen(in) * 2) + 1);
    while (*in != '\0') {
        switch (*in) {
            case '"':
                *work++ = '\\';
                *work++ = *in++;
                break;

            case '\\':
                // leave \u and \U alone
                if (*(in + 1) != 'u' && *(in + 1) != 'U') {
                    *work++ = '\\';
                }
                *work++ = *in++;
                break;

            default:
                *work++ = *in++;
                break;
        }
    }
    *work = '\0';

    return ret;
}

static void generate_loader(char *filename)
{
    table_t *table;
    table_t *hold_table;
    int i;

    hold_table = parse_table(filename);

    table = table_resize(hold_table, hold_table->max_key + 1, 1);

    printf(
        "\n"
        "/**\n"
        " * Generated from %s\n"
        " */\n\n",
        basename(filename)
    );

    //

    printf(
        "static table_row_t builtin_NEW_rows[%d] = {\n",
        table->used
    );

    for (i = 0; i < table->length; i++) {
        if (table->rows[i].key == 0) {
            continue;
        }

        printf(
            "    { "
            ".key  = 0x%04x, "
            ".data = \"%s\" "
            "},\n",
            table->rows[i].key,
            escape_string(table->rows[i].data)
        );
    }

    printf(
        "};\n\n"
    );

    //

    printf(
        "static table_t builtin_NEW_table = {\n"
        "    .length              = %d,\n"
        "    .used                = %d,\n"
        "    .max_data_length     = %d,\n"
        "    .max_key             = 0x%04x,\n"
        "    .hits                = 0,\n"
        "    .misses              = 0,\n"
        "    .seeks               = 0,\n"
        "    .overwrites          = %d,\n"
        "    .use_hash            = 0,\n"
        "    .builtin             = 1,\n"
        "    .rows                = builtin_NEW_rows,\n",
        table->used,
        table->used,
        table->max_data_length,
        table->max_key,
        table->overwrites
    );

    if (table->default_translation == NULL) {
        printf(
            "    .default_translation = NULL,\n"
        );
    } else {
        printf(
            "    .default_translation = \"%s\",\n",
            table->default_translation
        );
    }

    printf(
        "};\n\n"
    );

    //

    printf(
        "table_t *load_builtin_NEW_table(void)\n"
        "{\n"
        "    return table_resize(&builtin_NEW_table, %d, 1);\n"
        "}\n",
        ((int) ceil(table->used / BUILTIN_TABLE_MULTIPLE) + 1) * BUILTIN_TABLE_MULTIPLE
    );
}

int main(int argc, char **argv)
{
    if (argc == 1) {
        fprintf(stderr, "please specify a file to operate on\n");
        return -1;
    }

    generate_loader(argv[1]);

    return 0;
}

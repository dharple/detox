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

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <libgen.h>

#include "table.h"
#include "parse_table.h"
#include "builtin_table.h"

static void generate_loader(char *filename)
{
    struct translation_table *table;
    int escape;
    int i;

    table = parse_table(filename);

    printf(
        "\n"
        "/**\n"
        " * Generated from %s\n"
        " */\n\n",
        basename(filename)
    );

    //

    printf(
        "static struct translation_table_row builtin_NEW_rows[%d] = {\n",
        table->used
    );

    for (i = 0; i < table->length; i++) {
        if (table->rows[i].key == 0) {
            continue;
        }

        escape =
            (strcmp(table->rows[i].data, "\"") == 0) ||
            (strcmp(table->rows[i].data, "\\") == 0);

        printf(
            "    { "
            ".key  = 0x%04x, "
            ".data = \"%s%s\" "
            "},\n",
            table->rows[i].key,
            escape ? "\\" : "",
            table->rows[i].data
        );
    }

    printf(
        "};\n\n"
    );

    //

    printf(
        "static struct translation_table builtin_NEW_table = {\n"
        "    .length              = %d,\n"
        "    .used                = %d,\n"
        "    .max_data_length     = %d,\n"
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
        "struct translation_table *load_builtin_NEW_table(void)\n"
        "{\n"
        "    return table_resize(&builtin_NEW_table, %d, 1);\n"
        "}\n",
        ((int) ceil(table->used / BUILTIN_TABLE_MULTIPLE) + 1) * BUILTIN_TABLE_MULTIPLE
    );

    return;
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

/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <locale.h>

#include "table.h"
#include "wrapped.h"

#define LINE_LENGTH 6

enum {
    BASE_STATE,
    INSIDE_STATE
};

table_t *parse_table(char *filename)
{
    FILE *table_file;
    char *work;
    int code;
    int offset;
    char *parsed;
    int err;
    int size;
    int ret;
    int state;
    char *system_ctype;

    table_t *table;

    struct stat table_stat;

    err = stat(filename, &table_stat);
    if (err == -1) {
        return NULL;
    }

    system_ctype = setlocale(LC_CTYPE, "");
    if (system_ctype == NULL) {
        system_ctype = ""; // I don't think we can free the return from setlocale()
    }

    size = 0;

    if (table_stat.st_size > 0) {
        size = table_stat.st_size / LINE_LENGTH;
    }
#ifdef HAVE_STRUCT_STAT_ST_BLOCKS
    else {
        size = (512 * table_stat.st_blocks) / LINE_LENGTH;
    }
#endif

    if (size < 500) {
        size = 500;
    }

    table = table_init(size);
    if (table == NULL) {
        return NULL;
    }

    table_file = fopen(filename, "r");
    if (table_file == NULL) {
        fprintf(stderr, "Unable to open translation table: %s\n", strerror(errno));
        return NULL;
    }

    work = wrapped_malloc(1024);

    parsed = wrapped_malloc(1024);

    state = BASE_STATE;

    while (fgets(work, 1024, table_file) != NULL) {
        if (*work == '#') {

            /*
             * Don't even bother
             */
            continue;
        }

        parsed[0] = '\0';

        if (state == BASE_STATE) {
            ret = sscanf(work, " %s %n", parsed, &offset);

            if (ret == 0) {
                continue;
            }

            if (strncasecmp(parsed, "start", 5) == 0) {
                if (work[offset] == '\0') {
                    // All languages
                    state = INSIDE_STATE;
                    continue;
                }

                // check to see if a language specifier has been set

                if (work[offset] == '"') {
                    sscanf(work + offset + 1, "%[^\"]", parsed);
                } else if (work[offset] == '\'') {
                    sscanf(work + offset + 1, "%[^']", parsed);
                } else {
                    sscanf(work + offset, "%s", parsed);
                }

                if (strncasecmp(parsed, system_ctype, strlen(parsed)) == 0) {
                    state = INSIDE_STATE;
                }
                // else ignore this start/end block

                continue;
            }

            if (strncasecmp(parsed, "default", 7) == 0) {

                if (work[offset] == '\0') {
                    table->default_translation = NULL;
                    continue;
                }

                if (work[offset] == '"') {
                    sscanf(work + offset + 1, "%[^\"]", parsed);
                } else if (work[offset] == '\'') {
                    sscanf(work + offset + 1, "%[^']", parsed);
                } else {
                    sscanf(work + offset, "%s", parsed);
                }

                table->default_translation = wrapped_strdup(parsed);

                continue;
            }

            continue;
        }

        /*
         * Inside state
         */

        code = -1;

        ret = sscanf(work, "%i %n", &code, &offset);

        if (ret == 0 || code < 0 || offset < 0) {

            /*
             * Check for end
             */
            ret = sscanf(work, " %s %n", parsed, &offset);

            if (ret > 0 && strncasecmp(parsed, "end", 5) == 0) {
                state = BASE_STATE;
            }

            continue;
        }

        if (work[offset] == '\0') {
            continue;
        }

        if (work[offset] == '"') {
            sscanf(work + offset + 1, "%[^\"]", parsed);
        } else if (work[offset] == '\'') {
            sscanf(work + offset + 1, "%[^']", parsed);
        } else {
            sscanf(work + offset, "%s", parsed);
        }

        ret = table_put(table, code, parsed);
        if (ret == -1) {
            fprintf(stderr, "Failed to add row 0x%04x \"%s\" to translation table\n", code, parsed);
            table_free(table);
            fclose(table_file);
            free(work);
            free(parsed);
            return NULL;
        }
    }

    free(work);
    free(parsed);
    fclose(table_file);

    return table;
}

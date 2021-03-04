/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <stdio.h>

#include "detox_struct.h"

#include "config_file_dump.h"
#include "filelist.h"

void dump_config_file(config_file_t *config_file, options_t *main_options)
{
    sequence_t *sequence = NULL;
    filter_t *filter = NULL;
    char *file_walk;
    int count = 0;

    if (!main_options->verbose) {
        printf("available sequences:\n");
    }

    sequence = config_file->sequences;

    while (sequence != NULL) {
        if (main_options->verbose) {
            if (count++ > 0) {
                printf("\n");
            }
            printf("sequence name: ");
        } else {
            printf("\t");
        }
        printf("%s%s\n", sequence->name, (main_options->sequence_to_use == sequence) ? " (*)" : "");
        if (main_options->verbose) {
            printf("\tsource file: %s\n", sequence->source_filename);

            filter = sequence->filters;
            while (filter != NULL) {
                if (filter->cleaner == FILTER_UNCGI) {
                    printf("\tcleaner: uncgi\n");
                } else if (filter->cleaner == FILTER_SAFE) {
                    printf("\tcleaner: safe\n");
                } else if (filter->cleaner == FILTER_WIPEUP) {
                    printf("\tcleaner: wipeup\n");
                    printf("\t\tremove trailing: %s\n", filter->remove_trailing ? "yes" : "no");
                } else if (filter->cleaner == FILTER_ISO8859_1) {
                    printf("\tcleaner: iso8859_1\n");
                } else if (filter->cleaner == FILTER_UTF_8) {
                    printf("\tcleaner: utf_8\n");
                } else if (filter->cleaner == FILTER_MAX_LENGTH) {
                    printf("\tcleaner: max length\n");
                    printf("\t\tlength: %d\n", (unsigned int) filter->max_length);
                } else if (filter->cleaner == FILTER_LOWER) {
                    printf("\tcleaner: lower\n");
                }

                if (filter->builtin != NULL) {
                    printf("\t\tbuiltin table: %s\n", filter->builtin);
                } else if (filter->filename != NULL) {
                    printf("\t\ttranslation table: %s\n", filter->filename);
                }

                filter = filter->next;
            }
        }

        sequence = sequence->next;
    }

    if (filelist_count(config_file->files_to_ignore) > 0) {
        printf("\nfiles to ignore:\n");

        while ((file_walk = filelist_get(config_file->files_to_ignore))) {
            printf("\t%s\n", file_walk);
        }
    }
}

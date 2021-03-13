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

#include "detox_struct.h"

#include "config_file_dump.h"
#include "config_file.h"
#include "file.h"
#include "filelist.h"
#include "parse_options.h"
#include "sequence.h"

int main(int argc, char **argv)
{
    struct stat stat_info;
    int err;

    config_file_t *config_file = NULL;
    options_t *main_options;

    char *file_work = NULL;
    char *file_walk;

    main_options = parse_options_getopt(argc, argv);

    if (main_options == NULL) {
        fprintf(stderr, "detox: an error occurred while parsing command line arguments\n");
        exit(EXIT_FAILURE);
    }

    config_file = config_file_load(main_options);

    if (config_file == NULL) {
        fprintf(stderr, "detox: no config file to work with\n");
        exit(EXIT_FAILURE);
    }

    /*
     * Store the files_to_ignore array in the main_options struct for use in
     * parse_dir/file/special
     */

    main_options->files_to_ignore = config_file->files_to_ignore;

    /*
     * Determine which sequence to use
     */
    main_options->sequence_to_use = sequence_choose_default(config_file->sequences, main_options->sequence_name);

    /*
     * List sequences
     */
    if (main_options->list_sequences) {
        dump_config_file(config_file, main_options);
        exit(EXIT_SUCCESS);
    }

    /*
     * Fail if no sequence is available
     */
    if (main_options->sequence_to_use == NULL) {
        /*
         * XXX - Explain this better
         */
        fprintf(stderr, "detox: no sequence to work with\n");
        exit(EXIT_FAILURE);
    }

    /*
     * Check translation tables
     */

    sequence_review(main_options->sequence_to_use);

    /*
     * Do some actual work
     */

    if (!main_options->is_inline_mode) {
        while ((file_walk = filelist_get(main_options->files))) {
            if (main_options->verbose) {
                printf("Scanning: %s\n", file_walk);
            }

            err = lstat(file_walk, &stat_info);
            if (err == -1) {
                fprintf(stderr, "%s: %s\n", file_walk, strerror(errno));
            } else {
                if (S_ISDIR(stat_info.st_mode)) {
                    file_work = parse_file(file_walk, main_options);
                    parse_dir(file_work, main_options);
                    free(file_work);
                } else if (S_ISREG(stat_info.st_mode)) {
                    parse_file(file_walk, main_options);
                } else if (main_options->special) {
                    parse_file(file_walk, main_options);
                }
            }
        }
    } else {
        if (filelist_count(main_options->files) > 0) {
            while ((file_walk = filelist_get(main_options->files))) {
                err = lstat(file_walk, &stat_info);
                if (err == -1) {
                    fprintf(stderr, "%s: %s\n", file_walk, strerror(errno));
                } else {
                    if (S_ISDIR(stat_info.st_mode)) {
                        fprintf(stderr, "%s: is a directory\n", file_walk);
                    } else {
                        parse_inline(file_walk, NULL, main_options);
                    }
                }
            }
        } else {
            parse_inline(NULL, NULL, main_options);
        }
    }

    return 0;
}

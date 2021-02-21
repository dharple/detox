/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "detox.h"
#include "config_file.h"
#include "config_file_spoof.h"

/**
 * Loads a config file, any config file.
 *
 * @param struct detox_options *main_options The options from the command line.
 *
 * @return struct detox_parse_results
 */
struct detox_parse_results *config_file_load(struct detox_options *main_options)
{
    struct detox_parse_results *parse_results = NULL;
    char *check_config_file = NULL;
    char *file_work = NULL;
    int err;

    if (main_options->check_config_file) {
        check_config_file = strdup(main_options->check_config_file);
    }

    if (check_config_file != NULL) {
        parse_results = parse_config_file(check_config_file, NULL, main_options);
        if (parse_results == NULL) {
            fprintf(stderr, "detox: unable to open: %s\n", check_config_file);
            exit(EXIT_FAILURE);
        }
    } else {
        check_config_file = malloc(MAX_PATH_LEN);
        if (check_config_file == NULL) {
            fprintf(stderr, "out of memory: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

#ifdef SYSCONFDIR
        err = snprintf(check_config_file, MAX_PATH_LEN, "%s/detoxrc", SYSCONFDIR);
        if (err < MAX_PATH_LEN) {
            parse_results = parse_config_file(check_config_file, NULL, main_options);
        }
#endif

        if (parse_results == NULL) {
            parse_results = parse_config_file("/etc/detoxrc", NULL, main_options);
        }

        if (parse_results == NULL) {
            parse_results = parse_config_file("/usr/local/etc/detoxrc", NULL, main_options);
        }

        file_work = getenv("HOME");
        if (file_work != NULL) {
            err = snprintf(check_config_file, MAX_PATH_LEN, "%s/.detoxrc", file_work);
            if (err < MAX_PATH_LEN) {
                parse_results = parse_config_file(check_config_file, parse_results, main_options);
            }
        }

        if (parse_results == NULL) {
            parse_results = spoof_config_file();
        }

        free(check_config_file);
    }

    return parse_results;
}

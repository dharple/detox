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

#include "detox_struct.h"

#include "config_file.h"
#include "config_file_spoof.h"
#include "wrapped.h"

config_file_t *config_file_init(void)
{
    config_file_t *ret;

    ret = wrapped_malloc(sizeof(config_file_t));

    memset(ret, 0, sizeof(config_file_t));

    return ret;
}

/**
 * Loads a config file, any config file.
 *
 * @param options_t *main_options The options from the command line.
 *
 * @return config_file_t
 */
config_file_t *config_file_load(options_t *main_options)
{
    config_file_t *config_file = NULL;
    char *check_config_file = NULL;
    char *file_work = NULL;
    int err;

    if (main_options->check_config_file) {
        check_config_file = wrapped_strdup(main_options->check_config_file);
    }

    if (check_config_file != NULL) {
        config_file = parse_config_file(check_config_file, NULL, main_options);
        if (config_file == NULL) {
            fprintf(stderr, "detox: unable to open: %s\n", check_config_file);
            exit(EXIT_FAILURE);
        }
    } else {
        check_config_file = wrapped_malloc(MAX_PATH_LEN);

#ifdef SYSCONFDIR
        err = snprintf(check_config_file, MAX_PATH_LEN, "%s/detoxrc", SYSCONFDIR);
        if (err < MAX_PATH_LEN) {
            config_file = parse_config_file(check_config_file, NULL, main_options);
        }
#endif

        if (config_file == NULL) {
            config_file = parse_config_file("/etc/detoxrc", NULL, main_options);
        }

        if (config_file == NULL) {
            config_file = parse_config_file("/usr/local/etc/detoxrc", NULL, main_options);
        }

        file_work = getenv("HOME");
        if (file_work != NULL) {
            err = snprintf(check_config_file, MAX_PATH_LEN, "%s/.detoxrc", file_work);
            if (err < MAX_PATH_LEN) {
                config_file = parse_config_file(check_config_file, config_file, main_options);
            }
        }

        file_work = getenv("XDG_CONFIG_HOME");
        if (file_work != NULL) {
            err = snprintf(check_config_file, MAX_PATH_LEN, "%s/detox/detoxrc", file_work);
            if (err < MAX_PATH_LEN) {
                config_file = parse_config_file(check_config_file, config_file, main_options);
            }
        }

        if (config_file == NULL) {
            config_file = spoof_config_file();
        }

        free(check_config_file);
    }

    return config_file;
}

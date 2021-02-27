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

#include "detox_struct.h"

inline struct clean_string_options *new_clean_string_options()
{
    struct clean_string_options *ret;

    ret = malloc(sizeof(struct clean_string_options));
    if (ret == NULL) {
        fprintf(stderr, "detox: out of memory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    memset(ret, 0, sizeof(struct clean_string_options));

    return ret;
}

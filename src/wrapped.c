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
#include <errno.h>

#include "wrapped.h"

#ifdef SUPPORT_COVERAGE
int wrapped_malloc_failure = 0;
int wrapped_strdup_failure = 0;
#endif

void *wrapped_malloc(size_t size)
{
    void *ret;
    int err;

    ret = malloc(size);

#ifdef SUPPORT_COVERAGE
    if (wrapped_malloc_failure != 0) {
        free(ret);
        ret = NULL;
    }
#endif

    if (ret == NULL) {
        err = errno;
        fprintf(stderr, "detox: out of memory: %s\n", strerror(err));
        exit(EXIT_FAILURE);
    }

    return ret;
}

char *wrapped_strdup(const char *s)
{
    char *ret;
    int err;

    ret = strdup(s);

#ifdef SUPPORT_COVERAGE
    if (wrapped_strdup_failure != 0) {
        free(ret);
        ret = NULL;
    }
#endif

    if (ret == NULL) {
        err = errno;
        fprintf(stderr, "detox: out of memory: %s\n", strerror(err));
        exit(EXIT_FAILURE);
    }

    return ret;
}

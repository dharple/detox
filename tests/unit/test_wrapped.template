/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// need defines first
#include "config.h"

#include "wrapped.h"

#test-exit(1) test_wrapped_malloc_fail
#ifdef SUPPORT_COVERAGE
    wrapped_malloc_failure = 1;
    wrapped_malloc(5);
# else
    fprintf(stderr, "warning: not testing malloc failure\n");
    exit(EXIT_FAILURE);
#endif

#test-exit(1) test_wrapped_strdup_fail
#ifdef SUPPORT_COVERAGE
    wrapped_strdup_failure = 1;
    wrapped_strdup("test");
# else
    fprintf(stderr, "warning: not testing malloc failure\n");
    exit(EXIT_FAILURE);
#endif

#test test_wrapped_malloc
    void *test;

    test = wrapped_malloc(16);
    ck_assert((test != NULL));

#test test_wrapped_strdup
    char *check;

    check = wrapped_strdup("so broken");
    ck_assert_str_eq(check, "so broken");

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

#include "filelist.h"

#define STRLEN 128

struct test_definition {
    int pass;
    int count;
};

static struct test_definition tests[8] = {
    { .pass = 1, .count = 1 },
    { .pass = 2, .count = 2 },
    { .pass = 3, .count = 15 },
    { .pass = 4, .count = 16 },
    { .pass = 5, .count = 17 },
    { .pass = 6, .count = 1024 },
    { .pass = 7, .count = 100000 },
    { .pass = -1 }
};

#test test_filelist
    char *work;
    filelist_t *test_list;
    int i;
    int j;
    char *check, *last;

    work = alloca(STRLEN);
    last = NULL;

    for (i = 0; tests[i].pass != -1; i++) {
        test_list = filelist_init();

        for (j = 0; j < tests[i].count; j++) {
            snprintf(work, STRLEN, "test-%04d", j);
            filelist_put(test_list, work);
        }

        ck_assert_int_eq(filelist_count(test_list), tests[i].count);

        while ((check = filelist_get(test_list))) {
            last = check;
        }

        ck_assert_str_eq(last, work);

        // walk the pointer back to the beginning
        ck_assert_str_eq(filelist_get(test_list), "test-0000");
        if (tests[i].count > 1) {
            ck_assert_str_eq(filelist_get(test_list), "test-0001");
        }

        filelist_reset(test_list);

        // walk the pointer back to the beginning
        ck_assert_str_eq(filelist_get(test_list), "test-0000");

        ck_assert_int_eq(filelist_count(test_list), tests[i].count);

        filelist_free(test_list);
    }

#test test_filelist_get_null
    filelist_get(NULL);
    ck_assert_int_eq(filelist_count(NULL), 0);

#test test_filelist_get_empty
    char *work;
    filelist_t *test_list;

    test_list = filelist_init();

    work = filelist_get(test_list);

    ck_assert(work == NULL);

    ck_assert_int_eq(filelist_count(test_list), 0);

    filelist_free(test_list);

#test test_filelist_get_one
    filelist_t *test_list;

    test_list = filelist_init();

    filelist_put(test_list, "whatever");

    ck_assert_str_eq(filelist_get(test_list), "whatever");

    ck_assert(filelist_get(test_list) == NULL);

    ck_assert_str_eq(filelist_get(test_list), "whatever");

    ck_assert_int_eq(filelist_count(test_list), 1);

    filelist_free(test_list);

#test-exit(1) test_filelist_put_null
    filelist_put(NULL, NULL);

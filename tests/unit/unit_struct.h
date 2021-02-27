/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef __UNIT_STRUCT_H
#define __UNIT_STRUCT_H

struct test_filename {
    char *filename;
    char *expected;
    char *expected_a;
    char *expected_b;
    int   max_length;
};

#endif /* __UNIT_STRUCT_H */

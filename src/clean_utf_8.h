/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef __CLEAN_UTF_8_H
#define __CLEAN_UTF_8_H

#include "detox_struct.h"

#define UTF_8_MAX_LENGTH 6

extern char *clean_utf_8(char *filename, table_t *table);
extern int get_utf_8_width(char c);
extern int is_utf_8_cont(char c);
extern int is_utf_8_start(char c);

#endif /* __CLEAN_UTF_8_H */

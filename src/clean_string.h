/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef __CLEAN_STRING_H
#define __CLEAN_STRING_H

#include "detox_struct.h"

extern char *clean_iso8859_1(char *filename, table_t *table);
extern char *clean_safe(char *filename, table_t *table);
extern char *clean_uncgi(char *filename);
extern char *clean_wipeup(char *filename, int remove_trailing);
extern char *clean_max_length(char *filename, size_t max_length);
extern char *clean_lower(char *filename);

#endif /* __CLEAN_STRING_H */

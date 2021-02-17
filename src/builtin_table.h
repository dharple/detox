/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 *
 */

#ifndef __BUILTIN_TABLE_H
#define __BUILTIN_TABLE_H

#include "table.h"

#define BUILTIN_TABLE_MULTIPLE 256

extern struct translation_table *load_builtin_safe_table(void);
extern struct translation_table *load_builtin_iso8859_1_table(void);
extern struct translation_table *load_builtin_unicode_table(void);
extern struct translation_table *load_builtin_cp1252_table(void);

#endif /* __BUILTIN_TABLE_H */

/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef __TABLE_H
#define __TABLE_H

#include "detox_struct.h"

extern struct translation_table *table_init(int max_rows);
extern struct translation_table *table_resize(struct translation_table *table, int rows, int use_hash);
extern void table_free(struct translation_table *table);
extern int table_put(struct translation_table *table, unsigned int key, char *data);
extern char *table_get(struct translation_table *table, unsigned int key);

#endif /* __TABLE_H */

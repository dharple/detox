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

#define table_reset(t) t->hits = t->misses = t->seeks = 0;

extern table_t *table_init(int max_rows);
extern table_t *table_resize(table_t *table, int rows, int use_hash);
extern void table_free(table_t *table);
extern int table_put(table_t *table, unsigned int key, char *data);
extern char *table_get(table_t *table, unsigned int key);

#endif /* __TABLE_H */

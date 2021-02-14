/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef __TABLE_DUMP_H
#define __TABLE_DUMP_H

#include "table.h"

extern void table_dump(struct translation_table *table, int verbose);
extern void table_stats(struct translation_table *table);

#endif /*  __TABLE_DUMP_H */


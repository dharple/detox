/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef FILTER_H
#define FILTER_H

#include "detox_struct.h"

extern table_t *filter_find_table(const char *check_filename);
extern filter_t *filter_init(int cleaner);
extern table_t *filter_load_builtin_by_filename(const char *filename);
extern table_t *filter_load_builtin(filter_t *filter);
extern table_t *filter_load_table(filter_t *filter);
extern char *filter_run(filter_t *filter, char *work);

#endif //FILTER_H

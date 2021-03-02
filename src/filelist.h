/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef __FILELIST_H
#define __FILELIST_H

#include "detox_struct.h"

extern int filelist_count(filelist_t *list);
extern void filelist_free(filelist_t *list);
extern char *filelist_get(filelist_t *list);
extern filelist_t *filelist_init(void);
extern void filelist_put(filelist_t *list, const char *file);
extern void filelist_reset(filelist_t *list);

#endif /* __FILELIST_H */

/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef __WRAPPED_H
#define __WRAPPED_H

#include <stdio.h>

#ifdef SUPPORT_COVERAGE
extern int wrapped_malloc_failure;
extern int wrapped_strdup_failure;
#endif

extern void *wrapped_malloc(size_t size);
extern char *wrapped_strdup(const char *s);

#endif /* __WRAPPED_H */

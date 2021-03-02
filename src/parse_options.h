/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 *
 */

#ifndef __PARSE_OPTIONS_H
#define __PARSE_OPTIONS_H

#include "detox_struct.h"

extern char usage_message[];
extern char help_message[];

extern char usage_message_inline[];
extern char help_message_inline[];

extern options_t *options_init(void);
extern options_t *parse_options_getopt(int argc, char **argv);

#endif /* __PARSE_OPTIONS_H */

/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef __FILE_H
#define __FILE_H

#include "detox.h"

extern unsigned char *parse_file(unsigned char *filename, struct detox_options *options);

extern void parse_dir(unsigned char *indir, struct detox_options *options);

extern void parse_special(unsigned char *in, struct detox_options *options);

extern void parse_inline(unsigned char *filename, struct detox_options *options);

#endif				/* __FILE_H */

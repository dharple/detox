/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "detox_struct.h"

extern sequence_t *sequence_choose_default(sequence_t *sequences, const char *name);
extern sequence_t *sequence_init(const char *name);
extern void sequence_review(sequence_t *sequence);
extern char *sequence_run_filters(sequence_t *sequence, char *in);

#endif //SEQUENCE_H

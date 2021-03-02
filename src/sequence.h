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

extern struct detox_sequence_filter *sequence_choose_default(struct detox_sequence_list *sequences, char *sequence_name);
extern table_t *sequence_find_table(const char *check_filename);
extern table_t *sequence_load_builtin_by_filename(char *filename);
extern table_t *sequence_load_builtin(struct detox_sequence_filter *sequence);
extern table_t *sequence_load_table(struct detox_sequence_filter *sequence);
extern void sequence_review(struct detox_sequence_filter *sequence);

#endif //SEQUENCE_H

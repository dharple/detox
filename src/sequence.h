/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef DETOX_OPTIONS_H
#define DETOX_OPTIONS_H

extern struct detox_sequence_entry *sequence_choose_default(struct detox_sequence_list *sequences, char *sequence_name);
extern void sequence_review(struct detox_sequence_entry *sequence);

#endif //DETOX_OPTIONS_H

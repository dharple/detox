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

extern struct detox_sequence_entry *sequence_choose_default(struct detox_sequence_list *sequences, char *sequence_name);
extern struct translation_table *sequence_check_file(struct detox_sequence_entry *sequence);
extern struct translation_table *sequence_find_table(const char *check_filename);
extern struct translation_table *sequence_load_builtin_by_filename(char *filename);
extern struct translation_table *sequence_load_builtin(struct detox_sequence_entry *sequence);
extern void sequence_review(struct detox_sequence_entry *sequence);

#endif //SEQUENCE_H

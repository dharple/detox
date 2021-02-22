/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef __CLEAN_STRING_H
#define __CLEAN_STRING_H

#include <stdlib.h>

struct clean_string_options {
    char *filename;
    char *builtin;
    int remove_trailing;
    size_t max_length;
    void *translation_table;
};

extern char *clean_iso8859_1(char *filename, void *opts);
extern char *clean_safe(char *filename, void *opts);
extern char *clean_uncgi(char *filename, void *opts);
extern char *clean_wipeup(char *filename, void *opts);
extern char *clean_utf_8(char *filename, void *opts);
extern char *clean_max_length(char *filename, void *opts);
extern char *clean_lower(char *filename, void *opts);

#endif /* __CLEAN_STRING_H */

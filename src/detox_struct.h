/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef __DETOX_STRUCT_H
#define __DETOX_STRUCT_H

#include <stdlib.h>

#define MAX_PATH_LEN 256

enum {
    FILTER_ISO8859_1 = 1,
    FILTER_LOWER,
    FILTER_MAX_LENGTH,
    FILTER_SAFE,
    FILTER_UNCGI,
    FILTER_UTF_8,
    FILTER_WIPEUP
};

typedef struct {
    char **files;
    int max;
    int count;
    int ptr;
} filelist_t;

typedef struct {
    unsigned int key;
    char *data;
} table_row_t;

typedef struct {
    int length;
    int used;

    int max_data_length;

    char *default_translation;

    table_row_t *rows;

    int hits;
    int misses;
    int overwrites;
    int seeks;

    int use_hash;
    int builtin;
    int max_key;
} table_t;

typedef struct filter_t_ref {
    struct filter_t_ref *next;
    int cleaner;
    char *filename;
    char *builtin;
    int remove_trailing;
    size_t max_length;
    table_t *table;
} filter_t;

/*
 * Holds information about all of the defined sequences
 */
typedef struct sequence_t_ref {
    struct sequence_t_ref *next;
    char *name;
    filter_t *filters;
    char *source_filename;
} sequence_t;

/*
 * Holds the result of a config file parse
 */
typedef struct {
    sequence_t *sequences;
    filelist_t *files_to_ignore;
} config_file_t;

/**
 * Holds options that affect the entire operation of the program.
 */
typedef struct {
    int dry_run;
    int is_inline_bin;
    int is_inline_mode;
    int list_sequences;
    int recurse;
    int special;
    int verbose;

    sequence_t *sequence_to_use;
    filelist_t *files_to_ignore;

    char *sequence_name;

    char *check_config_file;

    filelist_t *files;
} options_t;

#endif /* __DETOX_STRUCT_H */

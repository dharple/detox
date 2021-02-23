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

struct translation_table_row {
    unsigned int key;
    char *data;
};

struct translation_table {
    int length;
    int used;

    int max_data_length;

    char *default_translation;

    struct translation_table_row *rows;

    int hits;
    int misses;
    int overwrites;
    int seeks;

    int use_hash;
    int builtin;
    int max_key;
};

struct clean_string_options {
    char *filename;
    char *builtin;
    int remove_trailing;
    size_t max_length;
    struct translation_table *translation_table;
};

/*
 * Holds information about all of the defined sequences
 */
struct detox_sequence_list {
    struct detox_sequence_list *next;

    char *name;

    /*
     * The top of the linked list of entries for this sequence
     */
    struct detox_sequence_entry *head;

    char *source_filename;
};

/*
 * Holds information about an entry within a specific sequence
 */
struct detox_sequence_entry {
    struct detox_sequence_entry *next;

    char *(*cleaner) (char *filename, struct clean_string_options *options);
    struct clean_string_options *options;
};

/*
 * Holds information about files the user has asked us to ignore
 */
struct detox_ignore_entry {
    struct detox_ignore_entry *next;

    char *filename;
};

/*
 * Holds the result of a config file parse
 */
struct detox_parse_results {
    struct detox_sequence_list *sequences;
    struct detox_ignore_entry *files_to_ignore;
};

/*
 * Holds command line options
 */
struct detox_options {
    int dry_run;
    int is_inline_bin;
    int is_inline_mode;
    int list_sequences;
    int recurse;
    int special;
    int verbose;

    struct detox_sequence_entry *sequence_to_use;
    struct detox_ignore_entry *files_to_ignore;

    char *sequence_name;

    char *check_config_file;

    char **files;
};

#endif /* __DETOX_STRUCT_H */

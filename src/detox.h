/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef __DETOX_H
#define __DETOX_H

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

	unsigned char *(*cleaner) (unsigned char *str, void *options);
	void *options;
};

/*
 * Holds information about files the user has asked us to ignore
 */
struct detox_ignore_entry {
	struct detox_ignore_entry *next;

	unsigned char *filename;
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
	int remove_trailing;
	int special;
	int verbose;

	struct detox_sequence_entry *sequence_to_use;
	struct detox_ignore_entry *files_to_ignore;

	char *sequence_name;

	char *check_config_file;

	char **files;
};

#endif				/* __DETOX_H */

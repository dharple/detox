/*
 * Copyright (c) 2004-2006, Doug Harple.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of author nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id: detox.h,v 1.12 2006/07/03 16:45:54 purgedhalo Exp $
 *
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
	int verbose;
	int recurse;
	int dry_run;
	int remove_trailing;
	int special;
	int list_sequences;

	struct detox_sequence_entry *sequence_to_use;
	struct detox_ignore_entry *files_to_ignore;

	char *sequence_name;

	char *check_config_file;

	char **files;
};

#endif				/* __DETOX_H */

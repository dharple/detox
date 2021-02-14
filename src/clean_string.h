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

struct clean_string_options {
	char *filename;
	int remove_trailing;
	size_t max_length;

	void *misc;
	void *translation_table;
};

extern char *clean_iso8859_1(char *s, void *opts);
extern char *clean_safe(char *s, void *opts);
extern char *clean_uncgi(char *s, void *opts);
extern char *clean_wipeup(char *s, void *opts);
extern char *clean_utf_8(char *s, void *opts);
extern char *clean_max_length(char *s, void *opts);
extern char *clean_lower(char *s, void *opts);

#endif				/* __CLEAN_STRING_H */

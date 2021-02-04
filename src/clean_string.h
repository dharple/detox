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

extern unsigned char *clean_iso8859_1_basic(unsigned char *s, void *options);
extern unsigned char *clean_iso8859_1(unsigned char *s, void *options);
extern unsigned char *clean_safe_basic(unsigned char *s, void *options);
extern unsigned char *clean_safe(unsigned char *s, void *options);
extern unsigned char *clean_uncgi(unsigned char *s, void *options);
extern unsigned char *clean_wipeup(unsigned char *s, void *options);
extern unsigned char *clean_utf_8_basic(unsigned char *s, void *options);
extern unsigned char *clean_utf_8(unsigned char *s, void *options);
extern unsigned char *clean_max_length(unsigned char *s, void *opts);
extern unsigned char *clean_lower(unsigned char *s, void *opts);

#endif				/* __CLEAN_STRING_H */

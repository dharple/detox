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
 * $Id: clean_string.h,v 1.17 2006/07/03 16:45:54 purgedhalo Exp $
 *
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

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
 * $Id: clean_string.c,v 1.24 2006/07/03 16:45:54 purgedhalo Exp $
 *
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "clean_string.h"

/* translation array for ISO8859.1 characters */
#include "iso8859_1.h"

/* translation array for unicode characters */
#include "unicode.h"

#include "parse_table.h"
#include "table.h"


/*
 * Translates ISO8859.1 characters (Latin-1) into lower ASCII characters.
 */
unsigned char *clean_iso8859_1_basic(unsigned char *s, void *opts)
{
	unsigned char *output, *input_walk, *output_walk, *replace_walk;
	int replace_pos;

	if (s == NULL) {
		return NULL;
	}

	output = malloc((strlen(s) * ISO8859_1_MAXLEN) + 1);
	if (output == NULL) {
		fprintf(stderr, "out of memory: %s\n", strerror(errno));
		return NULL;
	}

	input_walk = s;
	output_walk = output;

	while (*input_walk != '\0') {
		if (*input_walk >= ISO8859_1_OFFSET) {
			replace_pos = *input_walk - ISO8859_1_OFFSET;
			replace_walk = (unsigned char *)&iso8859_1_trans[replace_pos];

			while (*replace_walk != '\0') {
				*output_walk++ = *replace_walk++;
			}
			input_walk++;
		}
		else {
			*output_walk++ = *input_walk++;
		}
	}

	*output_walk = '\0';

	return output;
}

/*
 * Translates ISO8859.1 characters (Latin-1) into lower ASCII characters.
 */
unsigned char *clean_iso8859_1(unsigned char *s, void *opts)
{
	unsigned char *output, *input_walk, *output_walk, *replace_walk;

	struct translation_table *table = NULL;
	struct clean_string_options *options = NULL;

	if (s == NULL) {
		return NULL;
	}

	if (opts == NULL) {
		fprintf(stderr, "this shouldn't happen\n");
		exit(EXIT_FAILURE);
	}

	options = (struct clean_string_options *)opts;
	table = options->translation_table;

	output = malloc((strlen(s) * table->max_data_length) + 1);
	if (output == NULL) {
		fprintf(stderr, "out of memory: %s\n", strerror(errno));
		return NULL;
	}

	input_walk = s;
	output_walk = output;

	while (*input_walk != '\0') {
		if (*input_walk >= ISO8859_1_OFFSET) {
			replace_walk = table_get(table, *input_walk);
			if (replace_walk == NULL) {
				if (table->default_translation == NULL) {
					/*
					 * Null translation == leave it alone
					 */
					*output_walk++ = *input_walk++;
				}
				else {
					replace_walk = table->default_translation;
				}
			}

			if (replace_walk != NULL) {
				while (*replace_walk != '\0') {
					*output_walk++ = *replace_walk++;
				}
			}

			input_walk++;
		}
		else {
			*output_walk++ = *input_walk++;
		}
	}

	*output_walk = '\0';

	return output;
}


/*
 * Cleans up any unsafe characters.
 *
 * The rules are:
 *   Leave alone:
 *     - # ~ % ^ _ , . + =
 *
 *   Translate:
 *     &  into  _and_
 *
 *   Replace with _:
 *     ` ! @ $ * \ | : ; " ' < ? /
 *
 *   Replace with -:
 *     ( ) [ ] { }
 *
 */
unsigned char *clean_safe_basic(unsigned char *s, void *opts)
{
	unsigned char *output, *input_walk, *output_walk;

	if (s == NULL) {
		return NULL;
	}

	output = malloc((strlen(s) * 5) + 1);
	if (output == NULL) {
		fprintf(stderr, "out of memory: %s\n", strerror(errno));
		return NULL;
	}

	input_walk = s;
	output_walk = output;

	while (*input_walk != '\0') {
		if (isalnum(*input_walk)) {
			*output_walk++ = *input_walk++;
			continue;
		}

		switch (*input_walk) {
			case '-':
			case '#':
			case '~':
			case '%':
			case '^':
			case '_':
			case ',':
			case '.':
			case '+':
			case '=':
				*output_walk++ = *input_walk;
				break;

			case '&':
				*output_walk++ = '_';
				*output_walk++ = 'a';
				*output_walk++ = 'n';
				*output_walk++ = 'd';
				*output_walk++ = '_';
				break;

			case ' ':
			case '`':
			case '!':
			case '@':
			case '$':
			case '*':
			case '\\':
			case '|':
			case ':':
			case ';':
			case '"':
			case '\'':
			case '<':
			case '>':
			case '?':
			case '/':
				*output_walk++ = '_';
				break;

			case '(':
			case ')':
			case '[':
			case ']':
			case '{':
			case '}':
				*output_walk++ = '-';
				break;
		}

		input_walk++;
	}

	*output_walk = '\0';

	return output;
}


/*
 * Translates unsafe characters
 */
unsigned char *clean_safe(unsigned char *s, void *opts)
{
	unsigned char *output, *input_walk, *output_walk, *replace_walk;

	struct translation_table *table = NULL;
	struct clean_string_options *options = NULL;

	if (s == NULL) {
		return NULL;
	}

	if (opts == NULL) {
		fprintf(stderr, "this shouldn't happen\n");
		exit(EXIT_FAILURE);
	}

	options = (struct clean_string_options *)opts;
	table = options->translation_table;

	output = malloc((strlen(s) * table->max_data_length) + 1);
	if (output == NULL) {
		fprintf(stderr, "out of memory: %s\n", strerror(errno));
		return NULL;
	}

	input_walk = s;
	output_walk = output;

	while (*input_walk != '\0') {
		replace_walk = table_get(table, *input_walk);
		if (replace_walk == NULL) {
			if (table->default_translation == NULL) {

				/*
				 * Null translation == leave it alone
				 */
				*output_walk++ = *input_walk++;
			}
			else {
				replace_walk = table->default_translation;
			}
		}

		if (replace_walk != NULL) {
			while (*replace_walk != '\0') {
				*output_walk++ = *replace_walk++;
			}
		}

		input_walk++;
	}

	*output_walk = '\0';

	return output;
}



/*
 * Cleans up any CGI encoded characters, in the form "%" followed by 2 hex
 * digits.
 */
unsigned char *clean_uncgi(unsigned char *s, void *opts)
{
	unsigned char *output, *input_walk, *output_walk;
	unsigned char conv[3];

	if (s == NULL) {
		return NULL;
	}

	output = malloc(strlen(s) + 1);
	if (output == NULL) {
		fprintf(stderr, "out of memory: %s\n", strerror(errno));
		return NULL;
	}

	input_walk = s;
	output_walk = output;

	while (*input_walk != '\0') {
		if (input_walk[0] == '%' && isxdigit(input_walk[1]) && isxdigit(input_walk[2])) {
			conv[0] = input_walk[1];
			conv[1] = input_walk[2];
			conv[2] = 0;
			*output_walk++ = (unsigned char)strtol(conv, NULL, 16);
			input_walk += 3;
		}
		else {
			*output_walk++ = *input_walk++;
		}
	}

	*output_walk = '\0';

	return output;
}


/*
 * Reduces any series of "_" and "-" to a single character.  "-" takes
 * precedence.
 *
 * If "remove_trailing" is set to non-zero, then "." is added to the
 * comparison, and takes precedence.  This has the effect of reducing "-." or
 * "._", etc, to ".".
 *
 * Strips any "-", "_" or "#" from the beginning of a string.
 *
 */
unsigned char *clean_wipeup(unsigned char *s, void *opts)
{
	unsigned char *output, *input_walk, *output_walk;
	int matched;
	int remove_trailing;

	if (s == NULL) {
		return NULL;
	}

	remove_trailing = 0;
	if (opts != NULL) {
		remove_trailing = ((struct clean_string_options *)opts)->remove_trailing;
	}

	/* remove any -, _, or # at beginning of string */
	while (*s == '-' || *s == '_' || *s == '#') {
		s++;
	}

	output = malloc(strlen(s) + 1);
	if (output == NULL) {
		fprintf(stderr, "out of memory: %s\n", strerror(errno));
		return NULL;
	}

	input_walk = s;
	output_walk = output;
	matched = 0;

	while (*input_walk != '\0') {
		switch (*input_walk) {
			case '-':
				if (matched) {
					if (*output_walk == '_') {
						*output_walk = '-';
					}
				}
				else {
					*output_walk = '-';
				}

				matched = 1;
				break;

			case '_':
				if (!matched) {
					*output_walk = '_';
				}

				matched = 1;
				break;

			case '.':
				if (remove_trailing) {
					*output_walk = '.';
					matched = 1;
					break;
				}	/* else fall through */
			default:
				if (matched) {
					output_walk++;
					matched = 0;
				}

				*output_walk++ = *input_walk;
		}
		input_walk++;
	}

	if (matched) {
		output_walk++;
	}

	*output_walk = '\0';

	return output;
}

#define UTF_8_ENCODED 0x80
#define UTF_8_ENCODED_4_CHARS 0xf0
#define UTF_8_ENCODED_3_CHARS 0xe0
#define UTF_8_ENCODED_2_CHARS 0xc0

/*
 * Translates UTF-8 characters (Unicode Translation Format - 8 Bit) into
 * Unicode and then lower ASCII characters.
 */
unsigned char *clean_utf_8_basic(unsigned char *s, void *opts)
{
	unsigned char *output, *input_walk, *output_walk, *replace_walk;
	int new_value, expected_chars;

	if (s == NULL) {
		return NULL;
	}

	output = malloc((strlen(s) * UNICODE_MAXLEN) + 1);
	if (output == NULL) {
		fprintf(stderr, "out of memory: %s\n", strerror(errno));
		return NULL;
	}

	input_walk = s;
	output_walk = output;

	while (*input_walk != '\0') {
		if ((*input_walk & UTF_8_ENCODED) == 0) {
			*output_walk++ = *input_walk++;
			continue;
		}

		new_value = 0;
		expected_chars = 0;

		/*
		 * Needs to be done in descending orders due to the fact that
		 * the 2 char mask will match on the 4 char mask, but not
		 * vice versa.
		 */
		if ((*input_walk & UTF_8_ENCODED_4_CHARS) == UTF_8_ENCODED_4_CHARS) {

			/*
			 * 11110aaa 10bbbbbb 10cccccc 10dddddd
			 */

			new_value = *input_walk & 0x07;
			expected_chars = 3;
		}
		else if ((*input_walk & UTF_8_ENCODED_3_CHARS) == UTF_8_ENCODED_3_CHARS) {

			/*
			 * 1110aaaa 10bbbbbb 10cccccc
			 */

			new_value = *input_walk & 0x0f;
			expected_chars = 2;
		}
		else if ((*input_walk & UTF_8_ENCODED_2_CHARS) == UTF_8_ENCODED_2_CHARS) {

			/*
			 * 110aaaaa 10bbbbbb
			 */

			new_value = *input_walk & 0x1f;
			expected_chars = 1;
		}
		else {
			input_walk++;
			continue;
		}

		while (expected_chars > 0) {
			new_value <<= 6;

			input_walk++;

			if (*input_walk == '\0') {
				new_value = -1;
				break;
			}

			if ((*input_walk & UTF_8_ENCODED) == 0) {
				new_value = -1;
				break;
			}

			new_value += *input_walk & 0x3f;

			expected_chars--;
		}

		if (new_value == -1) {
			continue;
		}

		if (new_value >= UNICODE_COUNT) {
			*output_walk++ = '_';
			continue;
		}

		replace_walk = (unsigned char *)&unicode_trans[new_value];

		while (*replace_walk != '\0') {
			*output_walk++ = *replace_walk++;
		}
	}

	*output_walk = '\0';

	return output;
}

/*
 * Translates UTF-8 characters (Unicode Translation Format - 8 Bit) into
 * Unicode and then lower ASCII characters.
 */
unsigned char *clean_utf_8(unsigned char *s, void *opts)
{
	unsigned char *output, *input_walk, *output_walk, *replace_walk;
	int new_value, expected_chars;

	struct translation_table *table = NULL;
	struct clean_string_options *options = NULL;

	int characters_eaten;

	if (s == NULL) {
		return NULL;
	}

	if (opts == NULL) {
		fprintf(stderr, "this shouldn't happen\n");
		exit(EXIT_FAILURE);
	}

	options = (struct clean_string_options *)opts;
	table = options->translation_table;

	output = malloc((strlen(s) * table->max_data_length) + 1);
	if (output == NULL) {
		fprintf(stderr, "out of memory: %s\n", strerror(errno));
		return NULL;
	}

	input_walk = s;
	output_walk = output;

	while (*input_walk != '\0') {
		if ((*input_walk & UTF_8_ENCODED) == 0) {
			*output_walk++ = *input_walk++;
			continue;
		}

		new_value = 0;
		expected_chars = 0;
		characters_eaten = 0;

		/*
		 * Needs to be done in descending orders due to the fact that
		 * the 2 char mask will match on the 4 char mask, but not
		 * vice versa.
		 */
		if ((*input_walk & UTF_8_ENCODED_4_CHARS) == UTF_8_ENCODED_4_CHARS) {

			/*
			 * 11110aaa 10bbbbbb 10cccccc 10dddddd
			 */

			new_value = *input_walk & 0x07;
			expected_chars = 3;
			characters_eaten = 4;
		}
		else if ((*input_walk & UTF_8_ENCODED_3_CHARS) == UTF_8_ENCODED_3_CHARS) {

			/*
			 * 1110aaaa 10bbbbbb 10cccccc
			 */

			new_value = *input_walk & 0x0f;
			expected_chars = 2;
			characters_eaten = 3;
		}
		else if ((*input_walk & UTF_8_ENCODED_2_CHARS) == UTF_8_ENCODED_2_CHARS) {

			/*
			 * 110aaaaa 10bbbbbb
			 */

			new_value = *input_walk & 0x1f;
			expected_chars = 1;
			characters_eaten = 2;
		}
		else {
			input_walk++;
			continue;
		}

		while (expected_chars > 0) {
			new_value <<= 6;

			input_walk++;

			if (*input_walk == '\0') {
				new_value = -1;
				break;
			}

			if ((*input_walk & UTF_8_ENCODED) == 0) {
				new_value = -1;
				break;
			}

			new_value += *input_walk & 0x3f;

			expected_chars--;
		}

		if (new_value == -1) {
			continue;
		}

		replace_walk = table_get(table, new_value);

		if (replace_walk == NULL) {
			replace_walk = table->default_translation;
		}

		if (replace_walk == NULL) {

			/*
			 * Null translation == leave it alone
			 */
			*input_walk -= characters_eaten;

			while (characters_eaten) {
				*output_walk++ = *input_walk++;
				characters_eaten--;
			}

			continue;
		}

		while (*replace_walk != '\0') {
			*output_walk++ = *replace_walk++;
		}
	}

	*output_walk = '\0';

	return output;
}



/*
 * Trims a file down to specified length.
 */
unsigned char *clean_max_length(unsigned char *s, void *opts)
{
	unsigned char *output, *input_walk, *output_walk;
	size_t max_length;
	size_t s_length;
	size_t ext_length;

	if (s == NULL) {
		return NULL;
	}

	max_length = 256;
	if (opts != NULL) {
		max_length = ((struct clean_string_options *)opts)->max_length;
	}

	s_length = strlen(s);

	output = malloc(max_length + 1);
	if (output == NULL) {
		fprintf(stderr, "out of memory: %s\n", strerror(errno));
		return NULL;
	}

	snprintf(output, max_length + 1, "%s", s);

	if (s_length <= max_length) {
		return output;
	}

	input_walk = strrchr(s, '.');

	if (input_walk == NULL) {
		return output;
	}

	ext_length = strlen(input_walk);

	output_walk = output;
	output_walk += max_length - ext_length;

	while (*(output_walk - 1) == '.' && output_walk > output) {
		output_walk--;
	}

	snprintf(output_walk, ext_length + 1, "%s", input_walk);

	return output;
}


/*
 * Converts all characters to lowercase.
 */
unsigned char *clean_lower(unsigned char *s, void *opts)
{
	unsigned char *output, *input_walk, *output_walk;

	if (s == NULL) {
		return NULL;
	}

	output = malloc(strlen(s) + 1);
	if (output == NULL) {
		fprintf(stderr, "out of memory: %s\n", strerror(errno));
		return NULL;
	}

	input_walk = s;
	output_walk = output;

	while (*input_walk != '\0') {
		if (isupper(*input_walk)) {
			*output_walk++ = tolower(*input_walk++);
		}
		else {
			*output_walk++ = *input_walk++;
		}
	}

	*output_walk = '\0';

	return output;
}

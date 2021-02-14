/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "table.h"

/*
 * Internal function declarations
 */
static int table_hash1(int table_length, unsigned int key);
static int table_hash2(int table_length, unsigned int key);

struct translation_table *table_init(int max_rows)
{
	struct translation_table *ret;
	size_t row_length;

	if (max_rows <= 0) {
		max_rows = 500;
	}

	row_length = max_rows * sizeof(struct translation_table_row);

	ret = malloc(sizeof(struct translation_table));
	if (ret == NULL) {
		return NULL;
	}

	memset(ret, 0, sizeof(struct translation_table));

	ret->rows = malloc(row_length);
	if (ret->rows == NULL) {
		free(ret);
		return NULL;
	}

	memset(ret->rows, 0, row_length);

	ret->length = max_rows;
	ret->use_hash = 1;

	return ret;
}

struct translation_table *table_resize(struct translation_table *table, int rows)
{
	struct translation_table *ret;
	int i;

	ret = table_init(rows);

	if (ret == NULL)
		return table;

	if (table == NULL)
		return ret;

	if (table->default_translation != NULL) {
		ret->default_translation = strdup(table->default_translation);
	}

	ret->max_data_length = table->max_data_length;

	for (i = 0; i < table->length; i++) {
		if (table->rows[i].key > 0 && table->rows[i].data != NULL) {
			table_put(ret, table->rows[i].key, table->rows[i].data);
		}
	}

	return ret;
}

void table_free(struct translation_table *table)
{
	int i;

	if (table == NULL || table->builtin == 1) {
		return;
	}

	for (i = 0; i < table->length; i++) {
		if (table->rows[i].key > 0 && table->rows[i].data != NULL) {
			free(table->rows[i].data);
		}
	}

	free(table->rows);
	free(table);
}

static int table_hash1(int table_length, unsigned int key)
{
	return key % table_length;
}

static int table_hash2(int table_length, unsigned int key)
{
	int i;
	i = (key >> 8) % table_length;
	return (i == 0) ? 1 : i;
}

/**
 * Puts a row on the table.
 *
 * Key cannot be zero.
 *
 * @param struct translation_table * table The table to use.
 * @param unsigned int               key   The key for the table.
 * @param char *                     data  The data to store for the key.
 *
 * @return int The stored location, or -1 if an error occurred.
 */
int table_put(struct translation_table *table, unsigned int key, char *data)
{
	int offset;
	int seek;
	int i;

	if (table == NULL || key == 0) {
		return -1;
	}

	if (table->length == table->used) {
		return -1;
	}

	if (!table->use_hash) {
		offset = -1;

		for (i = 0 ; i < table->length ; i++) {
			if (table->rows[i].key == 0 || table->rows[i].key == key) {
				offset = i;
				break;
			}
		}

		if (offset == -1) {
			return -1;
		}
	} else {
		offset = table_hash1(table->length, key);

		if (table->rows[offset].key != 0 && table->rows[offset].key != key) {
			seek = table_hash2(table->length, key);
			while (table->rows[offset].key != 0 && table->rows[offset].key != key) {
				offset += seek;
				offset %= table->length;
			}
		}
	}

	if (table->rows[offset].key == key) {
		table->overwrites++;
	}

	table->rows[offset].key = key;
	table->rows[offset].data = strdup(data);
	table->used++;

	return offset;
}

/**
 * Gets a row from the table.
 *
 * Key cannot be zero.
 *
 * @param struct translation_table * table The table to use.
 * @param unsigned int               key   The key for the table.
 *
 * @return char * The value from the table, or NULL if it could not be found.
 */
char *table_get(struct translation_table *table, unsigned int key)
{
	int offset;
	int seek;
	int i;

	if (table == NULL || key == 0) {
		return NULL;
	}

	if (!table->use_hash) {
		offset = -1;

		for (i = 0 ; i < table->length ; i++) {
			if (table->rows[i].key == key) {
				offset = i;
				break;
			}
		}

		if (offset == -1) {
			return NULL;
		}
	} else {
		offset = table_hash1(table->length, key);

		if (table->rows[offset].key == 0) {
			return NULL;
		}

		if (table->rows[offset].key != key) {
			seek = table_hash2(table->length, key);
			while (table->rows[offset].key != key && table->rows[offset].key != 0) {
				table->misses++;

				offset += seek;
				offset %= table->length;
			}
		}

		if (table->rows[offset].key == 0) {
			return NULL;
		}
	}

	table->hits++;

	return table->rows[offset].data;
}

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

	if (table == NULL)
		return;

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
	return (key >> 8) % table_length;
}

/*
 * 0 is an invalid key (sorry)
 */
int table_put(struct translation_table *table, unsigned int key, char *data)
{
	int hashed;
	int hashed2;
	int i;

	if (table->length == table->used) {
		return -1;
	}

	hashed = table_hash1(table->length, key);

	if (table->rows[hashed].key != 0 && table->rows[hashed].key != key) {
		hashed2 = table_hash2(table->length, key);
		i = 0;
		while (table->rows[hashed].key != 0 && table->rows[hashed].key != key) {
			hashed += i + hashed2;
			hashed %= table->length;
		}
	}

	if (table->rows[hashed].key == key) {
		table->overwrites++;
	}

	table->rows[hashed].key = key;
	table->rows[hashed].data = strdup(data);
	table->used++;
	return hashed;
}

/*
 * 0 is an invalid key (sorry)
 */
char *table_get(struct translation_table *table, unsigned int key)
{
	int hashed;
	int hashed2;
	int i;

	if (table == NULL) {
		return NULL;
	}

	hashed = table_hash1(table->length, key);

	if (table->rows[hashed].key == 0) {
		return NULL;
	}

	if (table->rows[hashed].key != key) {
		hashed2 = table_hash2(table->length, key);
		i = 0;
		while (table->rows[hashed].key != key && table->rows[hashed].key != 0) {
			table->misses++;

			hashed += i + hashed2;
			hashed %= table->length;
		}
	}

	if (table->rows[hashed].key == 0) {
		return NULL;
	}

	table->hits++;

	return table->rows[hashed].data;
}

/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <stdlib.h>
#include <string.h>

#include "table.h"
#include "wrapped.h"

/*
 * Internal function declarations
 */
static int table_hash(int table_length, unsigned int key);

table_t *table_init(int max_rows)
{
    table_t *ret;
    size_t row_length;

    if (max_rows <= 0) {
        max_rows = 500;
    }

    row_length = max_rows * sizeof(table_row_t);

    ret = wrapped_malloc(sizeof(table_t));

    memset(ret, 0, sizeof(table_t));

    ret->rows = wrapped_malloc(row_length);

    memset(ret->rows, 0, row_length);

    ret->length = max_rows;
    ret->use_hash = 1;

    return ret;
}

table_t *table_resize(table_t *table, int rows, int use_hash)
{
    table_t *ret;
    int i;

    ret = table_init(rows);

    if (ret == NULL) {
        return table;
    }

    ret->use_hash = use_hash;

    if (table == NULL) {
        return ret;
    }

    if (table->default_translation != NULL) {
        ret->default_translation = wrapped_strdup(table->default_translation);
    }

    ret->overwrites = table->overwrites;

    for (i = 0; i < table->length; i++) {
        if (table->rows[i].key > 0 && table->rows[i].data != NULL) {
            table_put(ret, table->rows[i].key, table->rows[i].data);
        }
    }

    return ret;
}

void table_free(table_t *table)
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

    if (table->default_translation != NULL) {
        free(table->default_translation);
    }

    free(table->rows);
    free(table);
}

static int table_hash(int table_length, unsigned int key)
{
    return key % table_length;
}

/**
 * Puts a row on the table.
 *
 * Key cannot be zero.
 *
 * @param table_t * table The table to use.
 * @param unsigned int               key   The key for the table.
 * @param char *                     data  The data to store for the key.
 *
 * @return int The stored location, or -1 if an error occurred.
 */
int table_put(table_t *table, unsigned int key, char *data)
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

    offset = -1;

    if (table->use_hash) {
        seek = table_hash(table->length, key);

        if (table->rows[seek].key == 0 || table->rows[seek].key == key) {
            offset = seek;
        }
    }

    if (offset == -1) {
        for (i = 0; i < table->length; i++) {
            if (table->rows[i].key == 0 || table->rows[i].key == key) {
                offset = i;
                break;
            }
        }

        if (offset == -1) {
            return -1;
        }
    }

    if (table->rows[offset].key == key) {
        table->overwrites++;
    }

    table->rows[offset].key = key;
    table->rows[offset].data = wrapped_strdup(data);
    table->used++;

    if (table->max_key < key) {
        table->max_key = key;
    }

    if (table->max_data_length < strlen(data)) {
        table->max_data_length = strlen(data);
    }

    return offset;
}

/**
 * Gets a row from the table.
 *
 * Key cannot be zero.
 *
 * @param table The table to use.
 * @param key   The key for the table.
 *
 * @return The value from the table, or NULL if it could not be found.
 */
char *table_get(table_t *table, unsigned int key)
{
    int offset;
    int seek;
    int i;

    if (table == NULL || key == 0) {
        return NULL;
    }

    offset = -1;

    if (table->use_hash) {
        seek = table_hash(table->length, key);

        if (table->rows[seek].key == key) {
            offset = seek;
        }
    }

    if (offset == -1) {
        table->seeks++;

        for (i = 0; i < table->length; i++) {
            if (table->rows[i].key == key) {
                offset = i;
                break;
            }
        }

        if (offset == -1) {
            table->misses++;
            return NULL;
        }
    }

    table->hits++;

    return table->rows[offset].data;
}

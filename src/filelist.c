/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filelist.h"
#include "wrapped.h"

#define FILELIST_CHUNK 16

/**
 * Returns the current count of files in the list.
 *
 * @param list
 */
int filelist_count(filelist_t *list)
{
    if (list == NULL) {
        return 0;
    }

    return list->count;
}

/**
 * Releases memory currently held by a list.  Do *not* use the list after
 * calling this function on it.
 *
 * @param list
 */
void filelist_free(filelist_t *list)
{
    int i;

    if (list == NULL) {
        return;
    }

    for (i = 0; i < list->count; i++) {
        free(list->files[i]);
    }

    free(list->files);
    free(list);
}

/**
 * Retrieves the next member of the filelist.
 *
 * @param list
 *
 * @return
 */
char *filelist_get(filelist_t *list)
{
    if (list == NULL || list->count == 0) {
        return NULL;
    }

    if (list->ptr == list->count) {
        list->ptr = 0;
        return NULL;
    }

    return list->files[list->ptr++];
}

/**
 * Initializes a new filelist.
 *
 * @return
 */
filelist_t *filelist_init(void)
{
    filelist_t *list;

    list = wrapped_malloc(sizeof(filelist_t));
    memset(list, 0, sizeof(filelist_t));

    list->max = FILELIST_CHUNK;
    list->files = wrapped_malloc(sizeof(char *) * list->max);

    return list;
}

/**
 * Adds a file to a file list.
 *
 * @param list
 * @param file
 */
void filelist_put(filelist_t *list, const char *file)
{
    if (list == NULL) {
        fprintf(stderr, "detox: could not write to a null filelist\n");
        exit(EXIT_FAILURE);
    }

    if (list->count == list->max) {
        char **new_files;
        int new_max;
        size_t new_size;
        int i;

        new_max = list->max + FILELIST_CHUNK;
        new_size = sizeof(char *) * new_max;
        new_files = wrapped_malloc(new_size);
        memset(new_files, 0, new_size);

        for (i = 0; i < list->count; i++) {
            new_files[i] = list->files[i];
        }

        free(list->files);

        list->files = new_files;
        list->max = new_max;
    }

    list->files[list->count++] = wrapped_strdup(file);
    filelist_reset(list);
}

/**
 * Resets the internal pointer of a file list.
 *
 * @param list
 */
inline void filelist_reset(filelist_t *list)
{
    if (list == NULL) {
        return;
    }

    list->ptr = 0;
}

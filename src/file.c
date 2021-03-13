/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "config.h"

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

#include "clean_utf_8.h"
#include "file.h"
#include "filelist.h"
#include "sequence.h"
#include "wrapped.h"

/**
 * Determines if the file should be ignored
 *
 * @param filename The file to check
 * @param options  The main options
 *
 * @return bool
 */
static int ignore_file(const char *filename, const options_t *options)
{
    char *file_walk;

    if (filename[0] == '.') {
        return 1;
    }

    while ((file_walk = filelist_get(options->files_to_ignore))) {
        if (strcmp(filename, file_walk) == 0) {
            return 1;
        }
    }

    return 0;
}

/**
 * Whether or not the file is "." or ".."
 *
 * @param filename The file to check
 *
 * @return bool
 */
static int is_protected(const char *filename)
{
    if (filename == NULL) {
        return 0;
    }

    return (filename[0] == '.' && (filename[1] == '\0' || (filename[1] == '.' && filename[2] == '\0')));
}

/*
 * Renames file to a safe filename.
 */
char *parse_file(char *filename, options_t *options)
{
    char *old_filename, *old_filename_ptr, *new_filename;
    char *work, *hold;

    struct stat stat_info_old;
    struct stat stat_info_new;
    int err;
    size_t len;

    len = strlen(filename) + 1;
    old_filename = wrapped_malloc(len);

    memcpy(old_filename, filename, len);

    old_filename_ptr = strrchr(old_filename, '/');
    if (old_filename_ptr != NULL) {
        old_filename_ptr++;
    } else {
        old_filename_ptr = old_filename;
    }

    /*
     * Do the actual filename cleaning
     */

    work = wrapped_strdup(old_filename_ptr);

    if (is_protected(work)) {
        work = NULL;
    }

    hold = sequence_run_filters(options->sequence_to_use, work);
    if (work != NULL) {
        free(work);
    }
    work = hold;

    if (work == NULL) {
        return old_filename;
    }

    /* check to see if nothing changed */
    if (strcmp(old_filename_ptr, work) == 0) {
        return old_filename;
    }

    len = (old_filename_ptr - old_filename);
    new_filename = wrapped_malloc(len + strlen(work) + 1);

    strncpy(new_filename, old_filename, len);
    strcpy(new_filename + len, work);

    free(work);

    err = lstat(old_filename, &stat_info_old);
    if (err == -1) {
        free(new_filename);
        return old_filename;
    }

    err = lstat(new_filename, &stat_info_new);
    if (err != -1) { // New file exists
        if (stat_info_old.st_dev != stat_info_new.st_dev || // Different device
                stat_info_old.st_ino != stat_info_new.st_ino || // Different inode
                stat_info_old.st_nlink > 1) { // More than one hard link
            fprintf(stderr, "Cannot rename %s to %s: file already exists\n", old_filename, new_filename);

            free(new_filename);
            return old_filename;
        }
    }

    if (options->verbose || options->dry_run) {
        printf("%s -> %s\n", old_filename, new_filename);
    }

    if (options->dry_run) {
        free(new_filename);
        return old_filename;
    }

    err = rename(old_filename, new_filename);
    if (err == -1) {
        fprintf(stderr, "Cannot rename %s to %s: %s\n", old_filename, new_filename, strerror(errno));
        free(new_filename);
        return old_filename;
    }

    free(old_filename);

    return new_filename;
}

/*
 * Handles directory.
 */
void parse_dir(char *filename, options_t *options)
{
    char *new_file, *work;
    DIR *dir_handle;
    struct dirent *dir_entry;
    struct stat stat_info;
    int check_file;
    int err;
    size_t new_file_length;

    err = lstat(filename, &stat_info);
    if (err == -1) {
        return;
    }

    if (!S_ISDIR(stat_info.st_mode)) {
        return;
    }

    new_file_length = strlen(filename) + 1024;
    new_file = wrapped_malloc(new_file_length);

    /*
     * Parse directory
     */

    dir_handle = opendir(filename);
    if (dir_handle == NULL) {
        fprintf(stderr, "unable to parse: %s\n", strerror(errno));
        free(new_file);

        // too many open files
        if (errno == EMFILE) {
            exit(EXIT_FAILURE);
        }

        return;
    }

    dir_entry = readdir(dir_handle);

    while (dir_entry != NULL) {

        /*
         * Check for files that need to be ignored
         */
        check_file = !ignore_file(dir_entry->d_name, options);

        if (check_file) {
            snprintf(new_file, new_file_length, "%s/%s", filename, dir_entry->d_name);

            lstat(new_file, &stat_info);
            if (S_ISDIR(stat_info.st_mode)) {
                work = parse_file(new_file, options);
                if (options->recurse) {
                    parse_dir(work, options);
                }
                free(work);
            } else if (S_ISREG(stat_info.st_mode) || options->special) {
                work = parse_file(new_file, options);
                free(work);
            }
        }
        dir_entry = readdir(dir_handle);
    }
    closedir(dir_handle);
}

/**
 * Parses files in a stream, either read from STDIN, or a filename.  Writes the
 * results to a stream, either STDOUT, or a filename.
 *
 * @param in_filename  Filename to read from.  Set to NULL to use STDIN.
 * @param out_filename Filename to write to.  Set to NULL to use STDOUT.
 * @param options      Detox options.
 */
void parse_inline(char *in_filename, char *out_filename, options_t *options)
{
    FILE *in_fp;
    FILE *out_fp;
    size_t buf_size;
    size_t padding;

    char *base;
    char *hold;
    char *seek;
    char *work;
    int err;
    int has_newline;
    int remaining;

    if (in_filename != NULL) {
        if (!(in_fp = fopen(in_filename, "r"))) {
            fprintf(stderr, "%s: %s\n", in_filename, strerror(errno));
            return;
        }
    } else {
        in_fp = stdin;
    }

    if (out_filename != NULL) {
        if (!(out_fp = fopen(out_filename, "w"))) {
            fprintf(stderr, "%s: %s\n", out_filename, strerror(errno));
            return;
        }
    } else {
        out_fp = stdout;
    }

    buf_size = INLINE_BUF_SIZE;
    padding = INLINE_BUF_PADDING;
    base = wrapped_malloc(buf_size);

    while (fgets(base, buf_size - padding, in_fp)) {

        hold = strrchr(base, '\n');

        if (hold == NULL) {

#ifdef DEBUG
            fprintf(stderr, "detox: debug: fgets() didn't find a new line\n");
#endif

            //
            // Check to see if we stopped in the middle of a UTF-8 character.
            //

            hold = seek = strchr(base, '\0');
            seek--;

#ifdef DEBUG
            if (is_utf_8_cont(*seek)) {
                fprintf(stderr, "detox: debug: looks like we're in the middle of a UTF-8 character\n");
            }
#endif

            while (is_utf_8_cont(*seek) && (hold - seek) < UTF_8_MAX_LENGTH) {
#ifdef DEBUG
                fprintf(stderr, "detox: debug: at %02x, moving back one\n", (unsigned char) *seek);
#endif
                seek--;
            }

#ifdef DEBUG
            fprintf(stderr, "detox: debug: now at %02x\n", (unsigned char) *seek);
#endif

            if (is_utf_8_start(*seek)) {
#ifdef DEBUG
                fprintf(stderr, "detox: debug: at %02x, this is the start of a UTF-8 char, seeking ahead\n", (unsigned char) *seek);
#endif
                remaining = get_utf_8_width(*seek);
                do {
#ifdef DEBUG
                    fprintf(stderr, "detox: debug: at %02x\n", (unsigned char) *seek);
#endif
                    seek++;
                    remaining--;
                } while (remaining > 0 && is_utf_8_cont(*seek));

#ifdef DEBUG
                fprintf(stderr, "detox: debug: done with initial seek at %02x\n", (unsigned char) *seek);
#endif

                if (remaining > 0 && *seek == '\0') {
#ifdef DEBUG
                    fprintf(stderr, "detox: debug: we still need more data\n");
#endif

                    // try bringing bytes in one at a time
                    while (remaining > 0) {
                        err = fgetc(in_fp);
                        if (err == EOF) {
#ifdef DEBUG
                            fprintf(stderr, "detox: debug: hit EOF\n");
#endif
                            break;
                        }

                        *seek = err;

#ifdef DEBUG
                        fprintf(stderr, "detox: debug: read %02x\n", (unsigned char) *seek);
#endif

                        // but if it isn't a UTF-8 continuation byte
                        if (!is_utf_8_cont(*seek)) {
#ifdef DEBUG
                            fprintf(stderr, "detox: debug: %02x is not a UTF-8 continuation byte\n", (unsigned char) *seek);
#endif
                            // try to push it back on to the buffer
                            err = ungetc(*seek, in_fp);
                            if (err == EOF) {
                                fprintf(stderr, "detox: warning: stream push back failed\n");
                                seek++;
                            }
                            remaining = 0;
                            break;
                        }

                        seek++;
                        remaining--;
                    }
                    *seek = '\0';
                }

#ifdef DEBUG
                fprintf(stderr, "detox: debug: done with secondary seek at %02x\n", (unsigned char) *seek);
#endif
            }
        }

        // if we did fgetc(), and ungetc() failed, this might return
        // differently from before

        hold = strrchr(base, '\n');

        if (hold == NULL) {
            has_newline = 0;
        } else {
            has_newline = 1;
            *hold = '\0';
        }

        work = wrapped_strdup(base);

        if (is_protected(work)) {
            work = NULL;
        }

        hold = sequence_run_filters(options->sequence_to_use, work);
        if (work != NULL) {
            free(work);
        }
        work = hold;

        //
        // we're using printf("%s", ...) because don't know what is in the
        // string...
        //

        if (work != NULL) {
            fprintf(out_fp, "%s", work);
            free(work);
        } else {
            fprintf(out_fp, "%s", base);
        }

        if (has_newline) {
            fprintf(out_fp, "\n");
        }
    }

    if (in_filename != NULL) {
        fclose(in_fp);
    }

    if (out_filename != NULL) {
        fclose(out_fp);
    }
}

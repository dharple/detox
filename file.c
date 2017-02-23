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
 * $Id: file.c,v 1.15 2006/07/03 16:45:54 purgedhalo Exp $
 *
 */

#include "config.h"

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

#include "clean_string.h"
#include "file.h"
#include "detox.h"

static char badfiles[3][30] = {
	".",
	"..",
	""
};

/*
 * Internal function declarations
 */
static int ignore_file(unsigned char *filename, struct detox_options *options);

/*
 * Renames file to a safe filename.
 */
unsigned char *parse_file(unsigned char *filename, struct detox_options *options)
{
	unsigned char *old_filename, *old_filename_ptr, *new_filename;
	unsigned char *work, *hold;

	struct stat stat_info_old;
	struct stat stat_info_new;
	int err;
	size_t len;

	struct detox_sequence_entry *sequence;

	len = strlen(filename) + 1;
	old_filename = malloc(len);
	if (old_filename == NULL) {
		fprintf(stderr, "out of memory: %s\n", strerror(errno));
		return NULL;
	}
	memcpy(old_filename, filename, len);

	old_filename_ptr = strrchr(old_filename, '/');
	if (old_filename_ptr != NULL) {
		old_filename_ptr++;
	}
	else {
		old_filename_ptr = old_filename;
	}

	/*
	 * Check for files that need to be ignored
	 */

	if (ignore_file(old_filename_ptr, options)) {
		return old_filename;
	}

	/*
	 * Do the actual filename cleaning
	 */

	sequence = options->sequence_to_use;

	work = strdup(old_filename_ptr);

	while (sequence != NULL && work != NULL) {
		hold = sequence->cleaner(work, sequence->options);
		if (work != NULL) {
			free(work);
		}
		work = hold;

		sequence = sequence->next;
	}

	if (work == NULL) {
		return old_filename;
	}

	/* check to see if nothing changed */
	if (strcmp(old_filename_ptr, work) == 0) {
		return old_filename;
	}

	len = (old_filename_ptr - old_filename);
	new_filename = malloc(len + strlen(work) + 1);
	if (new_filename == NULL) {
		fprintf(stderr, "out of memory: %s\n", strerror(errno));
		free(work);
		free(old_filename);
		return NULL;
	}

	strncpy(new_filename, old_filename, len);
	strcpy(new_filename + len, work);

	free(work);

	err = lstat(old_filename, &stat_info_old);
	if (err == -1) {
		free(new_filename);
		return old_filename;
	}

	err = lstat(new_filename, &stat_info_new);
	if (err != -1) {	// New file exists
		if (stat_info_old.st_dev != stat_info_new.st_dev ||	// Different device
		    stat_info_old.st_ino != stat_info_new.st_ino ||	// Different inode
		    stat_info_old.st_nlink > 1)	// More than one hard link
		{
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
void parse_dir(unsigned char *indir, struct detox_options *options)
{
	unsigned char *new_file, *work;
	DIR *dir_handle;
	struct dirent *dir_entry;
	struct stat stat_info;
	int check_file;
	int err;
	size_t new_file_length;

	err = lstat(indir, &stat_info);
	if (err == -1) {
		return;
	}

	if (!S_ISDIR(stat_info.st_mode)) {
		return;
	}

	new_file_length = strlen(indir) + 1024;
	new_file = (char *)malloc(new_file_length);
	if (new_file == NULL) {
		fprintf(stderr, "out of memory: %s\n", strerror(errno));
		return;
	}

	/*
	 * Check to see if the directory is supposed to be ignored
	 */

	strcpy(new_file, indir);

	work = strrchr(new_file, '/');
	if (work != NULL) {
		work++;
		if (*work == '\0') {

			/*
			 * String ends with /, trim it off
			 */
			work--;
			*work = '\0';

			work = strrchr(new_file, '/');

			if (work != NULL) {
				work++;
			}
			else {
				work = new_file;
			}
		}
	}
	else {
		work = new_file;
	}

	if (ignore_file(work, options)) {
		free(new_file);
		return;
	}
	work = NULL;

	/*
	 * Parse directory
	 */

	dir_handle = opendir(indir);
	if (dir_handle == NULL) {
		fprintf(stderr, "unable to parse: %s\n", strerror(errno));
		free(new_file);
		return;
	}

	dir_entry = readdir(dir_handle);

	while (dir_entry != NULL) {

		/* 
		 * Check for files that need to be ignored
		 */
		check_file = !ignore_file(dir_entry->d_name, options);

		if (check_file) {
			snprintf(new_file, new_file_length, "%s/%s", indir, dir_entry->d_name);

			lstat(new_file, &stat_info);
			if (S_ISDIR(stat_info.st_mode)) {
				work = parse_file(new_file, options);
				if (options->recurse) {
					parse_dir(work, options);
				}
				free(work);
			}
			else if (S_ISREG(stat_info.st_mode)) {
				work = parse_file(new_file, options);
				free(work);
			}
			else if (options->special) {
				parse_special(new_file, options);
			}
		}
		dir_entry = readdir(dir_handle);
	}
	closedir(dir_handle);
}

/*
 * Handles a special file.
 */
void parse_special(unsigned char *in, struct detox_options *options)
{
	struct stat stat_info;
	char *new_file, *work;
	int err;

	/* detox, then parse_dir if a symlink to a dir */
	new_file = parse_file(in, options);
	if (!new_file) {
		return;
	}

	err = lstat(new_file, &stat_info);
	if (err == -1) {
		fprintf(stderr, "Unable to stat %s\n", in);
		free(new_file);
		return;
	}

	if (options->recurse && S_ISLNK(stat_info.st_mode)) {
		work = malloc(1024);
		if (!work) {
			fprintf(stderr, "out of memory: %s\n", strerror(errno));
			free(new_file);
			return;
		}

		memset(work, 0, 1024);
		err = readlink(new_file, work, 1023);
		if (err == -1) {
			fprintf(stderr, "Unable to read symbolic link %s\n", in);
			free(work);
			free(new_file);
		}

		err = lstat(work, &stat_info);
		if (err == -1) {
			fprintf(stderr, "Unable to follow symbolic link %s\n", in);
			free(work);
			free(new_file);
			return;
		}

		if (S_ISDIR(stat_info.st_mode)) {
			parse_dir(work, options);
		}

		free(work);
	}
	free(new_file);

}

/*
 * Determines if the file should be ignored
 */
static int ignore_file(unsigned char *filename, struct detox_options *options)
{
	struct detox_ignore_entry *ignore_walk;
	int x;

	for (x = 0; badfiles[x][0] != 0; x++) {
		if (strcmp(filename, badfiles[x]) == 0) {
			return 1;
		}
	}

	ignore_walk = options->files_to_ignore;
	while (ignore_walk != NULL) {
		if (strcmp(filename, ignore_walk->filename) == 0) {
			return 1;
		}
		ignore_walk = ignore_walk->next;
	}

	return 0;
}

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

#define BUF_SIZE 1024

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
			return;
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

/*
 * Renames file to a safe filename.
 */
void parse_inline(unsigned char *filename, struct detox_options *options)
{
	struct detox_sequence_entry *sequence;
	FILE *fp;
	unsigned char *base, *work, *hold;
	size_t buf_size;

	if (filename != NULL) {
		if (!(fp = fopen(filename, "r"))) {
			fprintf(stderr, "%s: %s\n", filename, strerror(errno));
			return;
		}
	}
	else {
		fp = stdin;
	}

	buf_size = BUF_SIZE;
	base = malloc(buf_size);
	if (base == NULL) {
		fprintf(stderr, "out of memory: %s\n", strerror(errno));
		return;
	}

	while (fgets(base, buf_size, fp)) {
		while (strrchr(base, '\n') == NULL) {
			work = realloc(base, buf_size + BUF_SIZE - 1);
			if (!fgets(work + buf_size - 1, BUF_SIZE, fp)) {
				base = work;
				break;
			}
			base = work;
			buf_size += BUF_SIZE - 1;
		}

		hold = strrchr(base, '\n');
		if (hold == NULL) {
			fprintf(stderr, "Unable to parse input\n");
			exit(EXIT_FAILURE);
		}
		*hold = '\0';

		work = strdup(base);

		sequence = options->sequence_to_use;

		while (sequence != NULL && work != NULL) {
			hold = sequence->cleaner(work, sequence->options);
			if (work != NULL) {
				free(work);
			}
			work = hold;

			sequence = sequence->next;
		}

		if (work != NULL) {
			printf("%s\n", work);
		}
		else {
			printf("\n");
		}
	}
}

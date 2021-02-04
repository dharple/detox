/**
 * This file is part of the Detox package.
 *
 * Copyright (c) Doug Harple <detox.dharple@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "config.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "detox.h"
#include "clean_string.h"

#include "file.h"

#include "config_file.h"
#include "config_file_spoof.h"
#include "config_file_dump.h"
#include "parse_table.h"
#include "parse_options.h"

#define MAX_PATH_LEN 256

int main(int argc, char **argv)
{
	struct stat stat_info;
	int err;

	struct detox_parse_results *parse_results = NULL;
	struct detox_sequence_list *list_work = NULL;
	struct detox_sequence_entry *which_sequence = NULL;
	struct detox_sequence_entry *work = NULL;
	struct detox_options *main_options;

	char *check_config_file = NULL;
	char *file_work = NULL;
	char **file_walk;

	main_options = parse_options_getopt(argc, argv);

	if (main_options == NULL) {
		fprintf(stderr, "detox: an error occurred while parsing command line arguments\n");
		exit(EXIT_FAILURE);
	}

	if (main_options->check_config_file) {
		check_config_file = strdup(main_options->check_config_file);
	}

	if (check_config_file != NULL) {
		parse_results = parse_config_file(check_config_file, NULL, main_options);
		if (parse_results == NULL) {
			fprintf(stderr, "detox: unable to open: %s\n", check_config_file);
			exit(EXIT_FAILURE);
		}
	}
	else {
		check_config_file = malloc(MAX_PATH_LEN);
		if (check_config_file == NULL) {
			fprintf(stderr, "out of memory: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}

#ifdef SYSCONFDIR
		err = snprintf(check_config_file, MAX_PATH_LEN, "%s/detoxrc", SYSCONFDIR);
		if (err < MAX_PATH_LEN)
			parse_results = parse_config_file(check_config_file, NULL, main_options);
#endif

		if (parse_results == NULL) {
			parse_results = parse_config_file("/etc/detoxrc", NULL, main_options);
		}

		if (parse_results == NULL) {
			parse_results = parse_config_file("/usr/local/etc/detoxrc", NULL, main_options);
		}

		file_work = getenv("HOME");
		if (file_work != NULL) {
			err = snprintf(check_config_file, MAX_PATH_LEN, "%s/.detoxrc", file_work);
			if (err < MAX_PATH_LEN)
				parse_results = parse_config_file(check_config_file, parse_results, main_options);

			file_work = NULL;
		}

		if (parse_results == NULL) {
			parse_results = spoof_config_file(main_options);
		}

		free(check_config_file);
	}

	if (parse_results == NULL) {
		fprintf(stderr, "detox: no config file to work with\n");
		exit(EXIT_FAILURE);
	}

	/*
	 * Store the files_to_ignore array in the main_options struct for use in
	 * parse_dir/file/special
	 */

	main_options->files_to_ignore = parse_results->files_to_ignore;

	/*
	 * Determine which sequence to use
	 */

	which_sequence = NULL;

	list_work = parse_results->sequences;

	while (list_work != NULL) {
		if (strcmp(list_work->name, (main_options->sequence_name == NULL) ? "default" : main_options->sequence_name) == 0) {
			which_sequence = list_work->head;
			break;
		}

		list_work = list_work->next;
	}

	/*
	 * If no sequence was found, and the user didn't specify a sequence
	 * to use, just use the first sequence.
	 */

	if (which_sequence == NULL && main_options->sequence_name == NULL) {
		if (parse_results->sequences != NULL) {
			which_sequence = parse_results->sequences->head;
		}
	}

	main_options->sequence_to_use = which_sequence;

	/*
	 * List sequences
	 */
	if (main_options->list_sequences) {
		dump_config_file(parse_results, main_options);
		exit(EXIT_SUCCESS);
	}

	/*
	 * Fail if no sequence is available
	 */
	if (main_options->sequence_to_use == NULL) {
		/*
		 * XXX - Explain this better
		 */
		fprintf(stderr, "detox: no sequence to work with\n");
		exit(EXIT_FAILURE);
	}

	/*
	 * Check translation tables
	 */

	work = main_options->sequence_to_use;
	while (work != NULL) {
		char *check_filename = NULL;
		int do_search = 0;

		struct translation_table *table = NULL;
		struct clean_string_options *opts;

		if (work->cleaner == &clean_iso8859_1) {
			if (work->options != NULL) {
				opts = work->options;
				if (opts->filename != NULL) {
					check_filename = opts->filename;
				}
			}

			if (!check_filename) {
				check_filename = "iso8859_1.tbl";
				do_search = 1;
			}
		}
		else if (work->cleaner == &clean_utf_8) {
			if (work->options != NULL) {
				opts = work->options;
				if (opts->filename != NULL) {
					check_filename = opts->filename;
				}
			}

			if (!check_filename) {
				check_filename = "unicode.tbl";
				do_search = 1;
			}
		}
		else if (work->cleaner == &clean_safe) {
			if (work->options != NULL) {
				opts = work->options;
				if (opts->filename != NULL) {
					check_filename = opts->filename;
				}
			}

			if (!check_filename) {
				check_filename = "safe.tbl";
				do_search = 1;
			}
		}

		if (check_filename || do_search) {

			table = NULL;

			if (do_search) {
				check_config_file = malloc(MAX_PATH_LEN);
				if (check_config_file == NULL) {
					fprintf(stderr, "out of memory: %s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}

#ifdef DATADIR
				err = snprintf(check_config_file, MAX_PATH_LEN, "%s/detox/%s", DATADIR, check_filename);
				if (err < MAX_PATH_LEN)
					table = parse_table(check_config_file);
#endif

				if (table == NULL) {
					err = snprintf(check_config_file, MAX_PATH_LEN, "/usr/share/detox/%s", check_filename);
					if (err < MAX_PATH_LEN)
						table = parse_table(check_config_file);
				}

				if (table == NULL) {
					err = snprintf(check_config_file, MAX_PATH_LEN, "/usr/local/share/detox/%s", check_filename);
					if (err < MAX_PATH_LEN)
						table = parse_table(check_config_file);
				}

				if (table == NULL) {

					/*
					 * Fall back to the non-file based
					 * cleaner
					 */
					if (work->cleaner == &clean_iso8859_1) {
						work->cleaner = &clean_iso8859_1_basic;
					}
					else if (work->cleaner == &clean_utf_8) {
						work->cleaner = &clean_utf_8_basic;
					}
					else if (work->cleaner == &clean_safe) {
						work->cleaner = &clean_safe_basic;
					}
					else {
						fprintf(stderr, "detox: unable to locate translation table or fall back\n");
						exit(EXIT_FAILURE);
					}
				}
				else {

					/*
					 * Allocate an options
					 */
					opts = malloc(sizeof(struct clean_string_options));
					if (opts == NULL) {
						fprintf(stderr, "out of memory: %s\n", strerror(errno));
						exit(EXIT_FAILURE);
					}
					memset(opts, 0, sizeof(struct clean_string_options));

					opts->translation_table = table;
					work->options = opts;
				}

				free(check_config_file);
			}
			else {
				table = parse_table(check_filename);
				if (table == NULL) {
					fprintf(stderr, "detox: unable to parse file: %s\n", check_filename);
					exit(EXIT_FAILURE);
				}

				opts = work->options;
				opts->translation_table = table;
			}
		}


		work = work->next;
	}

	/*
	 * Do some actual work
	 */

	if (!main_options->is_inline_mode) {
		file_walk = main_options->files;
		while (*file_walk) {
			if (main_options->verbose) {
				printf("Scanning: %s\n", *file_walk);
			}

			err = lstat(*file_walk, &stat_info);
			if (err == -1) {
				fprintf(stderr, "%s: %s\n", *file_walk, strerror(errno));
			}
			else {
				if (S_ISDIR(stat_info.st_mode)) {
					file_work = parse_file(*file_walk, main_options);
					parse_dir(file_work, main_options);
					free(file_work);
				}
				else if (S_ISREG(stat_info.st_mode)) {
					parse_file(*file_walk, main_options);
				}
				else if (main_options->special) {
					parse_special(*file_walk, main_options);
				}
			}

			file_walk++;
		}
	} else {
		if (main_options->files[0] != NULL) {
			file_walk = main_options->files;
			while (*file_walk) {
				err = lstat(*file_walk, &stat_info);
				if (err == -1) {
					fprintf(stderr, "%s: %s\n", *file_walk, strerror(errno));
				}
				else {
					if (S_ISDIR(stat_info.st_mode)) {
						fprintf(stderr, "%s: is a directory\n", *file_walk);
					}
					else {
						parse_inline(*file_walk, main_options);
					}
				}

				file_walk++;
			}
		}
		else {
			parse_inline(NULL, main_options);
		}
	}

	return 0;
}

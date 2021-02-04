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

#include "detox.h"
#include "config_file_dump.h"
#include "clean_string.h"

void dump_config_file(struct detox_parse_results *parse_results, struct detox_options *main_options)
{
	struct detox_sequence_list *list_work = NULL;
	struct detox_sequence_entry *work = NULL;
	struct detox_ignore_entry *ignore_walk = NULL;

	if (!main_options->verbose) {
		printf("available sequences:\n");
	}

	list_work = parse_results->sequences;

	while (list_work != NULL) {
		if (main_options->verbose) {
			printf("sequence name: ");
		}
		else {
			printf("\t");
		}
		printf("%s%s\n", list_work->name, (main_options->sequence_to_use == list_work->head) ? " (*)" : "");
		if (main_options->verbose) {
			printf("\tsource file: %s\n", list_work->source_filename);

			work = list_work->head;
			while (work != NULL) {
				if (work->cleaner == &clean_uncgi) {
					printf("\tcleaner: uncgi\n");
				}
				else if (work->cleaner == &clean_safe) {
					printf("\tcleaner: safe\n");
					if (work->options != NULL) {
						struct clean_string_options *opts = work->options;
						if (opts->filename != NULL) {
							printf("\t\ttranslation table: %s\n", opts->filename);
						}
					}
				}
				else if (work->cleaner == &clean_wipeup) {
					printf("\tcleaner: wipeup\n");
					if (work->options != NULL) {
						struct clean_string_options *opts = work->options;
						printf("\t\tremove trailing: %s\n", opts->remove_trailing ? "yes" : "no");
					}
				}
				else if (work->cleaner == &clean_iso8859_1) {
					printf("\tcleaner: iso8859_1\n");
					if (work->options != NULL) {
						struct clean_string_options *opts = work->options;
						if (opts->filename != NULL) {
							printf("\t\ttranslation table: %s\n", opts->filename);
						}
					}
				}
				else if (work->cleaner == &clean_utf_8) {
					printf("\tcleaner: utf_8\n");
					if (work->options != NULL) {
						struct clean_string_options *opts = work->options;
						if (opts->filename != NULL) {
							printf("\t\ttranslation table: %s\n", opts->filename);
						}
					}
				}
				else if (work->cleaner == &clean_max_length) {
					printf("\tcleaner: max length\n");
					if (work->options != NULL) {
						struct clean_string_options *opts = work->options;
						printf("\t\tlength: %d\n", (unsigned int)opts->max_length);
					}
				}
				if (work->cleaner == &clean_lower) {
					printf("\tcleaner: lower\n");
				}

				work = work->next;
			}
		}

		list_work = list_work->next;
	}


	if (parse_results->files_to_ignore) {
		printf("\nfiles to ignore:\n");

		ignore_walk = parse_results->files_to_ignore;

		while (ignore_walk != NULL) {
			printf("\t%s\n", ignore_walk->filename);
			ignore_walk = ignore_walk->next;
		}
	}

}

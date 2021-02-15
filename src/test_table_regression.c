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
#include <math.h>
#include <unistd.h>

#include "clean_string.h"
#include "parse_table.h"
#include "table_dump.h"

struct test_definition {
	int pass;
	int size;
	int multiple;
};

static struct test_definition tests[5] = {
	{ .pass = 1, .size = 0 },
	{ .pass = 2, .size = 10000 },
	{ .pass = 3, .size = 1000000 },
	{ .pass = 4, .size = 0, .multiple = 16 },
	{ .pass = -1 }
};

struct test_value {
	int value;
	char *expected;
};

static struct test_value values[9] = {
	{ .value = 0x0020, .expected = "space" },
	{ .value = 0x00A3, .expected = "_pound_" },
	{ .value = 0x014B, .expected = "ng" },
	{ .value = 0x0BAD, .expected = NULL },
	{ .value = 0x4000, .expected = NULL },
	{ .value = 0xC0DE, .expected = "code" },
	{ .value = 0xCAFE, .expected = "cafe" },
	{ .value = 0xFACE, .expected = "face" },
	{ .value = 0 }
};

static int regress_parse_table(int verbose)
{
	struct translation_table *table;
	char *check;
	int i, j;
	int size;
	int use_hash;

	if (verbose) {
		printf("loading table\n");
	}

	for (use_hash = 0; use_hash <= 1; use_hash++) {
		table = parse_table("regress.tbl");

		for (i = 0; tests[i].pass != -1; i++) {
			if (verbose) {
				printf("%s: pass = %d, use_hash = %d\n", __FUNCTION__, tests[i].pass, use_hash);
			}

			if (tests[i].size > 0) {
				if (verbose) {
					printf("resizing table to size %d\n", tests[i].size);
				}
				table = table_resize(table, tests[i].size, use_hash);
			} else if (tests[i].multiple > 0) {
				if (verbose) {
					printf("resizing table to multiple of %d\n", tests[i].multiple);
				}
				size = ((int) ceil(table->used/tests[i].multiple) + 1) * tests[i].multiple;
				if (verbose) {
					printf("resizing table to size %d\n", size);
				}
				table = table_resize(table, size, use_hash);
			}

			if (verbose) {
				printf("table length: %d\n", table->length);
				printf("table used: %d\n", table->used);
			}

			for (j = 0; values[j].value != 0; j++) {
				check = table_get(table, values[j].value);
				if (values[j].expected != NULL) {
					if (check == NULL || strcmp(check, values[j].expected) != 0) {
						if (verbose) {
							table_dump(table, 0);
						}
						fprintf(stderr, "ERROR: Unable to pull 0x%04x.  Expected %s.\n", values[j].value, values[j].expected);
						exit(1);
					}
					if (verbose) {
						printf("\t0x%04x -> \"%s\"\n", values[j].value, check);
					}
				} else {
					if (check != NULL) {
						if (verbose) {
							table_dump(table, 0);
						}
						fprintf(stderr, "ERROR: Pulled non-existent 0x%04x.\n", values[j].value);
						exit(1);
					}
					if (verbose) {
						printf("\t0x%04x -> NULL\n", values[j].value);
					}
				}
			}
		}
	}

	return 0;
}

int main(int argc, char **argv)
{
	int optcode;
	int verbose = 0;

	while ((optcode = getopt(argc, argv, "v")) != -1) {
		switch (optcode) {
			case 'h':
				printf("test-table-regression [-v]\n");
				return 0;

			case 'v':
				verbose = 1;
				break;

			default:
				fprintf(stderr, "unknown option: %c\n", optcode);
				return -1;
		}
	}

	regress_parse_table(verbose);

	return 0;
}

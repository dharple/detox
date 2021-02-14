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

#include "clean_string.h"
#include "parse_table.h"

int regress_parse_table(void) {
	struct translation_table *table;
	char *check;

	//
	// PASS ONE
	//

	printf("%s: pass 1\n", __FUNCTION__);

	table = parse_table("true_regress-1.tbl");
	printf("table length: %d\n", table->length);
	printf("table used: %d\n", table->used);

	check = table_get(table, 0xDEAD);
	if (check == NULL || strcmp(check, "dead") != 0)
		printf("ERROR: Unable to pull 0xDEAD\n");

	check = table_get(table, 0xBABE);
	if (check == NULL || strcmp(check, "babe") != 0)
		printf("ERROR: Unable to pull 0xBABE\n");

	check = table_get(table, 0xC0DE);
	if (check == NULL || strcmp(check, "code") != 0)
		printf("ERROR: Unable to pull 0xC0DE\n");

	check = table_get(table, 0x014B);
	if (check == NULL || strcmp(check, "ng") != 0)
		printf("ERROR: Unable to pull 0x014B\n");

	check = table_get(table, 0xFEEF);
	if (check != NULL)
		printf("ERROR: Pulled non-existant 0xFEEF\n");

	//
	// PASS TWO
	//

	printf("%s: pass 2\n", __FUNCTION__);

	table = table_resize(table, 10000);
	printf("table length: %d\n", table->length);
	printf("table used: %d\n", table->used);

	check = table_get(table, 0xDEAD);
	if (check == NULL || strcmp(check, "dead") != 0)
		printf("ERROR: Unable to pull 0xDEAD\n");

	check = table_get(table, 0xBABE);
	if (check == NULL || strcmp(check, "babe") != 0)
		printf("ERROR: Unable to pull 0xBABE\n");

	check = table_get(table, 0xC0DE);
	if (check == NULL || strcmp(check, "code") != 0)
		printf("ERROR: Unable to pull 0xC0DE\n");

	check = table_get(table, 0x014B);
	if (check == NULL || strcmp(check, "ng") != 0)
		printf("ERROR: Unable to pull 0x014B\n");

	check = table_get(table, 0xFEEF);
	if (check != NULL)
		printf("ERROR: Pulled non-existant 0xFEEF\n");

	//
	// PASS THREE
	//

	printf("%s: pass 3\n", __FUNCTION__);

	table = table_resize(table, 1000000);
	printf("table length: %d\n", table->length);
	printf("table used: %d\n", table->used);

	check = table_get(table, 0xDEAD);
	if (check == NULL || strcmp(check, "dead") != 0)
		printf("ERROR: Unable to pull 0xDEAD\n");

	check = table_get(table, 0xBABE);
	if (check == NULL || strcmp(check, "babe") != 0)
		printf("ERROR: Unable to pull 0xBABE\n");

	check = table_get(table, 0xC0DE);
	if (check == NULL || strcmp(check, "code") != 0)
		printf("ERROR: Unable to pull 0xC0DE\n");

	check = table_get(table, 0x014B);
	if (check == NULL || strcmp(check, "ng") != 0)
		printf("ERROR: Unable to pull 0x014B\n");

	check = table_get(table, 0xFEEF);
	if (check != NULL)
		printf("ERROR: Pulled non-existant 0xFEEF\n");

	//
	//
	//

	return 0;
}

int main(void)
{
	regress_parse_table();

	return 0;
}

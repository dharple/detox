#!/usr/bin/env bash
#
# This script generates the CP-1252 translation table based on a table found on
# Wikipedia which lists Unicode equivalents for CP-1252 characters.
#

PROJECT_ROOT=$(dirname "$(dirname "$(realpath "$0")")")
TABLEPATH="$PROJECT_ROOT/table"

TABLE1=$TABLEPATH/legacy/unicode.tbl
TABLE2=$TABLEPATH/legacy/unidecode.tbl

START_HEX=0x0080

#
# CP-1252 Translation - 0x0080-0x009F
#
# https://en.wikipedia.org/wiki/Windows-1252
#
# This list was pulled from the above article on Wikipedia on 2021-02-22.
# Simple regexes converted the lines to either "undef" or the first hex code in
# the table.
#

CHARS="
20AC
undef
201A
0192
201E
2026
2020
2021
02C6
2030
0160
2039
0152
undef
017D
undef

undef
2018
2019
201C
201D
2022
2013
2014
02DC
2122
0161
203A
0153
undef
017E
0178
"

CURRENT=$(printf "%d" "$START_HEX")

for CHAR in $CHARS ; do
	CURRENT_HEX=$(printf "0x%04X" "$CURRENT")

	if [ "$CHAR" = "undef" ] ; then
		echo "$CURRENT_HEX		\"-\"		# undefined, or control character"
	else
		CHECK=$(grep -c 0x"$CHAR" "$TABLE1")
		if [ "$CHECK" -eq "1" ] ; then
			echo -n "$CURRENT_HEX		\"\\u$CHAR\"	# "
			grep 0x"$CHAR" "$TABLE1" | sed -e"s/^.*# //"
		else
			echo -n "$CURRENT_HEX		\"\\u$CHAR\"	# "
			grep 0x"$CHAR" "$TABLE2" | sed -e"s/^.*# //"
		fi
	fi

	CURRENT=$((CURRENT + 1))
done

#!/usr/bin/env bash
#
# This script generates the CP-1252 translation table based on a table found on
# Wikipedia which lists Unicode equivalents for CP-1252 characters.
#

PROJECT_ROOT=$(dirname "$(dirname "$(realpath "$0")")")
TABLEPATH="$PROJECT_ROOT/table"

TABLE1=$TABLEPATH/legacy/unicode.tbl
TABLE2=$TABLEPATH/legacy/unidecode.tbl

START_HEX=0x00A0

#
# ISO-8859-1 Translation - 0x00A0-0x00FF
#

CHARS="
00A0
00A1
00A2
00A3
00A4
00A5
00A6
00A7
00A8
00A9
00AA
00AB
00AC
00AD
00AE
00AF

00B0
00B1
00B2
00B3
00B4
00B5
00B6
00B7
00B8
00B9
00BA
00BB
00BC
00BD
00BE
00BF

00C0
00C1
00C2
00C3
00C4
00C5
00C6
00C7
00C8
00C9
00CA
00CB
00CC
00CD
00CE
00CF

00D0
00D1
00D2
00D3
00D4
00D5
00D6
00D7
00D8
00D9
00DA
00DB
00DC
00DD
00DE
00DF

00E0
00E1
00E2
00E3
00E4
00E5
00E6
00E7
00E8
00E9
00EA
00EB
00EC
00ED
00EE
00EF

00F0
00F1
00F2
00F3
00F4
00F5
00F6
00F7
00F8
00F9
00FA
00FB
00FC
00FD
00FE
00FF
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
			grep ^0x"$CHAR" "$TABLE1" | sed -e"s/^.*# //"
		else
			echo -n "$CURRENT_HEX		\"\\u$CHAR\"	# "
			grep ^0x"$CHAR" "$TABLE2" | sed -e"s/^.*# //"
		fi
	fi

	CURRENT=$((CURRENT + 1))
done

#!/usr/bin/env bash
#
# Generate src/builtin_table.c
#

PROJECT_ROOT=$(dirname "$(dirname "$(realpath "$0")")")
cd "$PROJECT_ROOT" || exit

SRCDIR=$PROJECT_ROOT/src
GENERATE=$SRCDIR/generate-builtin-table

if [ ! -x "$GENERATE" ] ; then
	echo "please build $GENERATE first"
	exit 1
fi

cp "$SRCDIR"/builtin_table.c.in "$SRCDIR"/builtin_table.c

for TABLE in safe iso8859_1 unicode cp1252 ; do
	echo "process builtin $TABLE"
	$GENERATE "$PROJECT_ROOT"/table/$TABLE.tbl | sed -e"s/NEW/$TABLE/" >> "$SRCDIR"/builtin_table.c
done

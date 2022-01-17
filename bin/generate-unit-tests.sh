#!/usr/bin/env bash
#
# Generate tests/unit/*.c from .ts
#

PROJECT_ROOT=$(dirname "$(dirname "$(realpath "$0")")")
cd "$PROJECT_ROOT" || exit

TESTDIR=$PROJECT_ROOT/tests/unit
cd "$TESTDIR" || exit

CHECKMK=$(command -v checkmk)

if [ ! -x "$CHECKMK" ] ; then
	echo "Please install checkmk, possibly through the 'check' package, before using this"
	exit 1
fi

BASE=/tmp/detoxtest/
if [ ! -d "$BASE" ] ; then
	mkdir "$BASE"
fi
WORK=$(realpath "$(mktemp -d "$BASE"/work-XXXXXX)")

for FILE in *.template ; do
	INPUT=$(basename "$FILE")
	OUTPUT="${INPUT%.template}.c"
	echo -n "process check file $INPUT... "
	$CHECKMK "$INPUT" > "$WORK"/"$OUTPUT"
	sed -i -e 's/^ . Edit the original.*/\0\n * Run `make internals` from the base of the project to regenerate this file./' "$WORK"/"$OUTPUT"
	if [ ! -f "$OUTPUT" ] ; then
		echo "created"
		mv "$WORK"/"$OUTPUT" "$OUTPUT"
	else
		diff -q "$WORK"/"$OUTPUT" "$OUTPUT" > /dev/null 2>&1
		if [ "$?" -eq "0" ] ; then
			echo
			rm "$WORK"/"$OUTPUT"
		else
			echo "updated"
			mv "$OUTPUT" "$OUTPUT".bak
			mv "$WORK"/"$OUTPUT" "$OUTPUT"
		fi
	fi
done

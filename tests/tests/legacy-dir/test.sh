#!/usr/bin/env bash
#
# Subset of legacy regression tests: recursion
#

set -e

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. $TESTBASE/test-functions

DETOX=$1

BASE=/tmp/detoxtest/
if [ ! -d $BASE ] ; then
	mkdir $BASE
fi
WORK=$(realpath $(mktemp -d $BASE/test-custom-XXXXXX))

# -----

cd $WORK

mkdir 1 2 3 4 5

INPUT="1/&ampersand"
touch "$INPUT"

INPUT="2/|bar"
touch "$INPUT"

INPUT="3/:colon"
touch "$INPUT"

INPUT="4/,comma"
touch "$INPUT"

INPUT="5/#octothorpe"
touch "$INPUT"

INPUT="%percent"
touch "$INPUT"

INPUT="+plus"
touch "$INPUT"

INPUT=";semi colon"
touch "$INPUT"

# -----

cd $TESTBASE
$DETOX -r $WORK
cd $WORK

# -----

EXPECTED="1/and_ampersand 2/bar 3/colon 4/,comma 5/octothorpe %percent +plus semi_colon"

for OUTPUT in $EXPECTED ; do
	if [ ! -f "$OUTPUT" ] ; then
		echo expected "$OUTPUT" not found
		exit 1
	fi
done


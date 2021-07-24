#!/usr/bin/env bash
#
# Tests to confirm GitHub issue #56 is resolved
#
# https://github.com/dharple/detox/issues/56
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

# ------------------------------------------------------------------------

WORK=$(realpath $(mktemp -d $BASE/test-custom-XXXXXX))
cd $WORK

VALGRIND=$(command -v valgrind || true)

touch a b c d e f g h i j k

if [ -z "$VALGRIND" ] ; then
	echo WARNING: valgrind is not installed
else
	$VALGRIND --quiet --error-exitcode=1 --track-origins=yes $DETOX a b c d e f g h i j k
fi

# ------------------------------------------------------------------------

INPUT="has space"
OUTPUT="has_space"

for COUNT in 10 100 1000 ; do
	WORK=$(realpath $(mktemp -d $BASE/test-custom-XXXXXX))
	cd $WORK

	touch $(seq 1 $COUNT) "$INPUT"
	$DETOX $(seq 1 $COUNT) "$INPUT"

	if [ ! -f "$OUTPUT" ] ; then
		echo "Could not find expected file '$OUTPUT'"
		exit 1
	fi
done

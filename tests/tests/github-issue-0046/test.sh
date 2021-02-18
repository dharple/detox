#!/bin/bash -e
#
# Confirms that GitHub issue #41 is fixed
#
# https://github.com/dharple/detox/issues/41
#

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. $TESTBASE/test-functions

DETOX=$1
TABLEPATH=$(dirname $TESTBASE)/table

# ---------------------------------------------------------------------------

INPUT="safe.txt"
OUTPUT=$INPUT
METHOD1=max-length-32

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

# ---------------------------------------------------------------------------

INPUT="safe and stuff.txt"
OUTPUT="safe and.txt"
METHOD1=max-length-12

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

# ---------------------------------------------------------------------------

INPUT="safe and stuff.txt"
OUTPUT="safe.txt"
METHOD1=max-length-8

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

# ---------------------------------------------------------------------------

INPUT="safe.tar.gz"
OUTPUT=$INPUT
METHOD1=max-length-32

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

# ---------------------------------------------------------------------------

INPUT="safe and stuff.tar.gz"
OUTPUT="safe .tar.gz"
METHOD1=max-length-12

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

# ---------------------------------------------------------------------------

INPUT="safe and stuff.tar.gz"
OUTPUT="s.tar.gz"
METHOD1=max-length-8

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

#!/usr/bin/env bash
#
# Tests to confirm GitHub issue #9 is resolved
#
# https://github.com/dharple/detox/issues/9
#

set -e

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. $TESTBASE/test-functions

DETOX=$1
TABLEPATH=$(dirname $TESTBASE)/table
INPUT=$(printf "hi\tthere")
OUTPUT="hi_there"

# ---------------------------------------------------------------------------

METHOD1=safe

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

# ---------------------------------------------------------------------------

METHOD1=safe-basic

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

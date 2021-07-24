#!/usr/bin/env bash
#
# Tests to confirm GitHub issue #17 is resolved
#
# https://github.com/dharple/detox/issues/17
#

set -e

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. $TESTBASE/test-functions

DETOX=$1
TABLEPATH=$(dirname $TESTBASE)/table
INPUT=$(printf "new\nline")
OUTPUT="new_line"

# ---------------------------------------------------------------------------

METHOD1=safe

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

# ---------------------------------------------------------------------------

METHOD1=safe-basic

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

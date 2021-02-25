#!/usr/bin/env -S bash -e
#
# Confirm that GitHub issue #33 is fixed
#
# https://github.com/dharple/detox/issues/33
#

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. $TESTBASE/test-functions

DETOX=$1
TABLEPATH=$(dirname $TESTBASE)/table
METHOD1=utf_8

# ---------------------------------------------------------------------------

INPUT="safe string"
OUTPUT="safe string"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

# ---------------------------------------------------------------------------

if [ "$LOWER_UNICODE_SUPPORT" = "yes" ] ; then
	INPUT=$(printf "\\u00AE reg")
	OUTPUT="_reg_ reg"

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"
else
	echo "WARNING: skipped one test due to old version of bash"
fi

# ---------------------------------------------------------------------------

INPUT=$(printf "\\U00010348 hwair")
OUTPUT="hu hwair"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

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

. $TESTBASE/test-functions.sh
. $TESTBASE/character-helper.sh

DETOX=$1
TABLEPATH=$(dirname $TESTBASE)/table
METHOD1=utf_8

# ---------------------------------------------------------------------------

INPUT="safe string"
OUTPUT="safe string"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

# ---------------------------------------------------------------------------

INPUT=$(printf "${UTF8_00AE} reg")
OUTPUT="_reg_ reg"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

# ---------------------------------------------------------------------------

INPUT=$(printf "${UTF8_10348} hwair")
OUTPUT="hu hwair"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

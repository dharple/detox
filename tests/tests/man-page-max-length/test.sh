#!/usr/bin/env bash
#
# Confirm a statement on the detoxrc.5 manpage
#

set -e

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. $TESTBASE/test-functions.sh
. $TESTBASE/character-helper.sh

DETOX=$1
TABLEPATH=$(dirname $TESTBASE)/table
METHOD1=max-length-12

# ---------------------------------------------------------------------------

INPUT="this_is_my_file.txt"
OUTPUT="this_is_.txt"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"


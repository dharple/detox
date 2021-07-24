#!/usr/bin/env bash
#
# Tests to confirm GitHub issue #14 is resolved
#
# https://github.com/dharple/detox/issues/14
#

set -e

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. $TESTBASE/test-functions

# Run the test based on data from
# https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=861537

DETOX=$1
INPUT="mÉ Æ.txt"
OUTPUT="mÉ_Æ.txt"
METHOD="utf_8"
TABLE="test.tbl"

test_single_table "$DETOX" "$INPUT" "$OUTPUT" "$METHOD" "$TABLE"

# ---------------------------------------------------------------------------

# Run the same test, based on the default table

OUTPUT="mE_AE.txt"
TABLE=$(dirname $TESTBASE)/table/unicode.tbl.sample

test_single_table "$DETOX" "$INPUT" "$OUTPUT" "$METHOD" "$TABLE"

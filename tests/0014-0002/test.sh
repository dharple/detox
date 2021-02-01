#!/bin/bash -e
#
# Test to confirm GitHub issue #14 is resolved
#
# https://github.com/dharple/detox/issues/14
#

BASE=$(dirname $(dirname $(realpath $0)))
. $BASE/test-functions

# ---------------------------------------------------------------------------

# Run the test based on the default table

DETOX=$1
INPUT="mÉ Æ.txt"
OUTPUT="mE_AE.txt"
METHOD="utf_8"
TABLE=$(dirname $BASE)/table/unicode.tbl.sample

test_single_table "$DETOX" "$INPUT" "$OUTPUT" "$METHOD" "$TABLE"

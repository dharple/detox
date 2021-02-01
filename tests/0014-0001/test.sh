#!/bin/bash -e
#
# Test to confirm GitHub issue #14 is resolved
#
# https://github.com/dharple/detox/issues/14
#

BASE=$(dirname $(dirname $(realpath $0)))
. $BASE/test-functions

# Run the test based on data from 
# https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=861537

DETOX=$1
INPUT="mÉ Æ.txt"
OUTPUT="mÉ_Æ.txt"
METHOD="utf_8"
TABLE="test.tbl"

test_single_table "$DETOX" "$INPUT" "$OUTPUT" "$METHOD" "$TABLE"

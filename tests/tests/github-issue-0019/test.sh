#!/usr/bin/env bash
#
# Tests to confirm GitHub issue #19 is resolved
#
# https://github.com/dharple/detox/issues/19
#

set -e

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. $TESTBASE/test-functions

# Run the test based on data from the issue

DETOX=$1
INPUT="01 5G Core Networks.pdf"
OUTPUT="01 5G Core Networks.pdf"
METHOD="safe"
TABLE="test.tbl"

test_single_table "$DETOX" "$INPUT" "$OUTPUT" "$METHOD" "$TABLE"

#!/usr/bin/env bash
#
# Subset of legacy regression tests: max_32
#

set -e

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. $TESTBASE/test-functions

DETOX=$1
TABLEPATH=$(dirname $TESTBASE)/table
METHOD1=safe
METHOD2=wipeup
METHOD3=max-length-32

# ---------------------------------------------------------------------------

INPUT="___________underscore_______________________x"
OUTPUT="underscore_x"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

# ---------------------------------------------------------------------------

INPUT="..........how.....does.....this....translate......"
OUTPUT="..........how.....does.....this."

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

# ---------------------------------------------------------------------------

INPUT="_-_-_-_-_-dotted-_-_-_-_line.....part......two......."
OUTPUT="dotted-line.....part......two."

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

# ---------------------------------------------------------------------------

INPUT="registered trademark symbol, ®, is a typographic symbol"
OUTPUT="registered_trademark_symbol,_®,"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"


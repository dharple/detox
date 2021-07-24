#!/usr/bin/env bash
#
# Subset of legacy regression tests: remove_trailing
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
METHOD2=wipeup-remove-trailing

# ---------------------------------------------------------------------------

INPUT="___________underscore_______________________.x"
OUTPUT="underscore.x"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

# ---------------------------------------------------------------------------

INPUT="..........how.....does.....this....translate......"
OUTPUT=".how.does.this.translate."

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

# ---------------------------------------------------------------------------

INPUT="_-_-_-_-_-dotted-_-_-_-_line.....part......two......."
OUTPUT="dotted-line.part.two."

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

# ---------------------------------------------------------------------------

INPUT="blah-.-de_._da"
OUTPUT="blah.de.da"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"


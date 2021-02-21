#!/bin/bash -e
#
# Confirms that GitHub issue #21 is fixed
#
# https://github.com/dharple/detox/issues/21
#

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. $TESTBASE/test-functions

DETOX=$1
TABLEPATH=$(dirname $TESTBASE)/table

# used for test_detoxrc
DETOXRC=$(dirname $TESTBASE)/etc/detoxrc
SEQUENCE=default

# used for test_sequence
METHOD1=safe-basic
METHOD2=wipeup

# ---------------------------------------------------------------------------

INPUT="safe string"
OUTPUT="safe_string"

test_detoxrc "$DETOX" "$INPUT" "$OUTPUT" "$DETOXRC" "$TABLEPATH" "$SEQUENCE"
test_detoxrc "$DETOX" "$INPUT" "$OUTPUT" "$DETOXRC" "$TABLEPATH"
test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT=$(printf "\\u00AE reg")
OUTPUT=$(printf "\\u00AE_reg")

test_detoxrc "$DETOX" "$INPUT" "$OUTPUT" "$DETOXRC" "$TABLEPATH" "$SEQUENCE"
test_detoxrc "$DETOX" "$INPUT" "$OUTPUT" "$DETOXRC" "$TABLEPATH"
test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT=$(printf "\\u00A9 copy")
OUTPUT=$(printf "\\u00A9_copy")

test_detoxrc "$DETOX" "$INPUT" "$OUTPUT" "$DETOXRC" "$TABLEPATH" "$SEQUENCE"
test_detoxrc "$DETOX" "$INPUT" "$OUTPUT" "$DETOXRC" "$TABLEPATH"
test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT=$(printf "\\u00C6 capital AE")
OUTPUT=$(printf "\\u00C6_capital_AE")

test_detoxrc "$DETOX" "$INPUT" "$OUTPUT" "$DETOXRC" "$TABLEPATH" "$SEQUENCE"
test_detoxrc "$DETOX" "$INPUT" "$OUTPUT" "$DETOXRC" "$TABLEPATH"
test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT=$(printf "\\u00DE capital thorn")
OUTPUT=$(printf "\\u00DE_capital_thorn")

test_detoxrc "$DETOX" "$INPUT" "$OUTPUT" "$DETOXRC" "$TABLEPATH" "$SEQUENCE"
test_detoxrc "$DETOX" "$INPUT" "$OUTPUT" "$DETOXRC" "$TABLEPATH"
test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT=$(printf "\\u014A capital ENG")
OUTPUT=$(printf "\\u014A_capital_ENG")

test_detoxrc "$DETOX" "$INPUT" "$OUTPUT" "$DETOXRC" "$TABLEPATH" "$SEQUENCE"
test_detoxrc "$DETOX" "$INPUT" "$OUTPUT" "$DETOXRC" "$TABLEPATH"
test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT=$(printf "\\u0172 capital U with Ogonek")
OUTPUT=$(printf "\\u0172_capital_U_with_Ogonek")

test_detoxrc "$DETOX" "$INPUT" "$OUTPUT" "$DETOXRC" "$TABLEPATH" "$SEQUENCE"
test_detoxrc "$DETOX" "$INPUT" "$OUTPUT" "$DETOXRC" "$TABLEPATH"
test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

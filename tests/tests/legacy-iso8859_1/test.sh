#!/usr/bin/env bash
#
# Subset of legacy regression tests: iso8859_1
#

set -e

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. $TESTBASE/test-functions

DETOX=$1
TABLEPATH=$(dirname $TESTBASE)/table
METHOD1=iso8859_1
METHOD2=safe
METHOD3=wipeup

# ---------------------------------------------------------------------------

INPUT="safe string"
OUTPUT="safe_string"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

# ---------------------------------------------------------------------------

INPUT=$(printf "\\xAE reg")
OUTPUT="reg_reg"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

# ---------------------------------------------------------------------------

INPUT=$(printf "\\xA9 copy")
OUTPUT="copy_copy"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

# ---------------------------------------------------------------------------

INPUT=$(printf "\\xC6 capital AE")
OUTPUT="AE_capital_AE"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

# ---------------------------------------------------------------------------

INPUT=$(printf "\\xDE capital thorn")
OUTPUT="TH_capital_thorn"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"


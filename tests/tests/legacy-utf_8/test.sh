#!/usr/bin/env bash
#
# Subset of legacy regression tests: utf8
#

set -e

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. $TESTBASE/test-functions

LC_ALL=en_US.UTF-8
export LC_ALL

DETOX=$1
TABLEPATH=$(dirname $TESTBASE)/table
METHOD1=utf_8
METHOD2=safe
METHOD3=wipeup

# ---------------------------------------------------------------------------

INPUT="safe string"
OUTPUT="safe_string"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

# ---------------------------------------------------------------------------

INPUT=$(printf "\\u00AE reg")
OUTPUT="reg_reg"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

# ---------------------------------------------------------------------------

INPUT=$(printf "\\u00A9 copy")
OUTPUT="copy_copy"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

# ---------------------------------------------------------------------------

INPUT=$(printf "\\u00C6 capital AE")
OUTPUT="AE_capital_AE"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

# ---------------------------------------------------------------------------

INPUT=$(printf "\\u00DE capital thorn")
OUTPUT="TH_capital_thorn"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

# ---------------------------------------------------------------------------

INPUT=$(printf "\\u014A capital ENG")
OUTPUT="NG_capital_ENG"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

# ---------------------------------------------------------------------------

INPUT=$(printf "\\u0172 capital U with Ogonek")
OUTPUT="U_capital_U_with_Ogonek"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"


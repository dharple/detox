#!/usr/bin/env bash
#
# Subset of legacy regression tests: UTF-8
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
METHOD1=utf_8
METHOD2=safe
METHOD3=wipeup

# ---------------------------------------------------------------------------

INPUT="safe string"
OUTPUT="safe_string"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

# ---------------------------------------------------------------------------

INPUT=$(printf "${UTF8_00AE} reg")
OUTPUT="reg_reg"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

# ---------------------------------------------------------------------------

INPUT=$(printf "${UTF8_00A9} copy")
OUTPUT="copy_copy"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

# ---------------------------------------------------------------------------

INPUT=$(printf "${UTF8_00C6} capital AE")
OUTPUT="AE_capital_AE"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

# ---------------------------------------------------------------------------

INPUT=$(printf "${UTF8_00DE} capital thorn")
OUTPUT="TH_capital_thorn"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

# ---------------------------------------------------------------------------

INPUT=$(printf "${UTF8_014A} capital ENG")
OUTPUT="NG_capital_ENG"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

# ---------------------------------------------------------------------------

INPUT=$(printf "${UTF8_0172} capital U with Ogonek")
OUTPUT="U_capital_U_with_Ogonek"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"


#!/usr/bin/env -S bash -e
#
# Subset of legacy regression tests: iso8859_1
#

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. $TESTBASE/test-functions.sh
. $TESTBASE/character-helper.sh

DETOX=$1
TABLEPATH=$(dirname $TESTBASE)/table
METHOD1=iso8859_1-basic
METHOD2=safe-basic
METHOD3=wipeup

# ---------------------------------------------------------------------------

INPUT="safe string"
OUTPUT="safe_string"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

# ---------------------------------------------------------------------------

INPUT=$(printf "${CHAR_AE} reg")
OUTPUT="reg_reg"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

# ---------------------------------------------------------------------------

INPUT=$(printf "${CHAR_A9} copy")
OUTPUT="copy_copy"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

# ---------------------------------------------------------------------------

INPUT=$(printf "${CHAR_C6} capital AE")
OUTPUT="AE_capital_AE"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

# ---------------------------------------------------------------------------

INPUT=$(printf "${CHAR_DE} capital thorn")
OUTPUT="TH_capital_thorn"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"


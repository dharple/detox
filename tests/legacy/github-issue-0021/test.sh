#!/usr/bin/env bash
#
# Confirms that GitHub issue #21 is fixed
#
# https://github.com/dharple/detox/issues/21
#

set -e

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. "$TESTBASE"/test-functions.sh
. "$TESTBASE"/character-helper.sh

DETOX=$1
TABLEPATH=$(dirname $(dirname "$TESTBASE"))/table

# used for test_detoxrc
DETOXRC=$(dirname $(dirname "$TESTBASE"))/etc/detoxrc
SEQUENCE=default

# used for test_sequence
METHOD1=safe-basic
METHOD2=wipeup

# ---------------------------------------------------------------------------

INPUT="safe string"
OUTPUT="safe_string"

test_detoxrc "$DETOX" "$INPUT" "$OUTPUT" "$DETOXRC" "$SEQUENCE"
test_detoxrc "$DETOX" "$INPUT" "$OUTPUT" "$DETOXRC"
test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT=$(printf "${UTF8_00AE} reg")
OUTPUT=$(printf "${UTF8_00AE}_reg")

test_detoxrc "$DETOX" "$INPUT" "$OUTPUT" "$DETOXRC" "$SEQUENCE"
test_detoxrc "$DETOX" "$INPUT" "$OUTPUT" "$DETOXRC"
test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT=$(printf "${UTF8_00A9} copy")
OUTPUT=$(printf "${UTF8_00A9}_copy")

test_detoxrc "$DETOX" "$INPUT" "$OUTPUT" "$DETOXRC" "$SEQUENCE"
test_detoxrc "$DETOX" "$INPUT" "$OUTPUT" "$DETOXRC"
test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT=$(printf "${UTF8_00C6} capital AE")
OUTPUT=$(printf "${UTF8_00C6}_capital_AE")

test_detoxrc "$DETOX" "$INPUT" "$OUTPUT" "$DETOXRC" "$SEQUENCE"
test_detoxrc "$DETOX" "$INPUT" "$OUTPUT" "$DETOXRC"
test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT=$(printf "${UTF8_00DE} capital thorn")
OUTPUT=$(printf "${UTF8_00DE}_capital_thorn")

test_detoxrc "$DETOX" "$INPUT" "$OUTPUT" "$DETOXRC" "$SEQUENCE"
test_detoxrc "$DETOX" "$INPUT" "$OUTPUT" "$DETOXRC"
test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT=$(printf "${UTF8_014A} capital ENG")
OUTPUT=$(printf "${UTF8_014A}_capital_ENG")

test_detoxrc "$DETOX" "$INPUT" "$OUTPUT" "$DETOXRC" "$SEQUENCE"
test_detoxrc "$DETOX" "$INPUT" "$OUTPUT" "$DETOXRC"
test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT=$(printf "${UTF8_0172} capital U with Ogonek")
OUTPUT=$(printf "${UTF8_0172}_capital_U_with_Ogonek")

test_detoxrc "$DETOX" "$INPUT" "$OUTPUT" "$DETOXRC" "$SEQUENCE"
test_detoxrc "$DETOX" "$INPUT" "$OUTPUT" "$DETOXRC"
test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

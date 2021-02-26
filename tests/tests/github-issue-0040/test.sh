#!/usr/bin/env -S bash -e
#
# Confirms that GitHub issue #40 is fixed
#
# https://github.com/dharple/detox/issues/40
#

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. $TESTBASE/test-functions.sh
. $TESTBASE/character-helper.sh

DETOX=$1
TABLEPATH=$(dirname $TESTBASE)/table

for METHOD1 in utf_8 utf_8-basic ; do

	# ---------------------------------------------------------------------------

	INPUT="_ () whatever"
	OUTPUT=$INPUT

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

	# ---------------------------------------------------------------------------

	INPUT=$(printf "${UTF8_4000} ignore")
	OUTPUT="_ ignore"

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

	# ---------------------------------------------------------------------------

	INPUT=$(printf "${UTF8_007F} delete")
	OUTPUT="_ delete"

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

	# ---------------------------------------------------------------------------

	# 2 byte UTF-8 encoded ${CHAR_00} (null)

	INPUT=$(printf "${CHAR_C0}${CHAR_80} null")
	OUTPUT="_hidden_null_ null"

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

	# ---------------------------------------------------------------------------

	# 2 byte UTF-8 encoded ${CHAR_20} (space)

	INPUT=$(printf "${CHAR_C0}${CHAR_A0} space")
	OUTPUT="  space"

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

	# ---------------------------------------------------------------------------

	# 2 byte UTF-8 encoded ${CHAR_30} (zero)

	INPUT=$(printf "${CHAR_C0}${CHAR_B0} zero")
	OUTPUT="0 zero"

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

	# ---------------------------------------------------------------------------

	# 2 byte UTF-8 encoded ${CHAR_7E} (tilde)

	INPUT=$(printf "${CHAR_C1}${CHAR_BE} tilde")
	OUTPUT="~ tilde"

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

	# ---------------------------------------------------------------------------

	# 2 byte UTF-8 encoded ${CHAR_7F} (delete)

	INPUT=$(printf "${CHAR_C1}${CHAR_BF} delete")
	OUTPUT="_ delete"

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

done

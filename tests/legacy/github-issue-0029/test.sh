#!/usr/bin/env bash
#
# Confirms that GitHub issue #29 is fixed
#
# https://github.com/dharple/detox/issues/29
#

set -e

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. $TESTBASE/test-functions.sh
. $TESTBASE/character-helper.sh

DETOX=$1
TABLEPATH=$(dirname $0)
METHOD2=wipeup

for METHOD1 in safe safe-basic ; do

	# ---------------------------------------------------------------------------

	INPUT="  safe string"
	OUTPUT="safe_string"

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

	# ---------------------------------------------------------------------------

	INPUT=$(printf "${CHAR_07} bell")
	if [ "$METHOD1" = "safe" ] ; then
		OUTPUT="beep_bell"
	else
		OUTPUT="bell"
	fi

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

	# ---------------------------------------------------------------------------

	# $CHAR_0A would not work here
	INPUT=$(printf "${CHAR_09} tab \n new line")
	if [ "$METHOD1" = "safe" ] ; then
		OUTPUT="tab_tab_nl_new_line"
	else
		OUTPUT="tab_new_line"
	fi

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

	# ---------------------------------------------------------------------------

	INPUT=$(printf "${UTF8_00C6} capital AE")
	OUTPUT=$(printf "${UTF8_00C6}_capital_AE")

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

	# ---------------------------------------------------------------------------

	INPUT=$(printf "${CHAR_C6} capital AE")
	OUTPUT=$(printf "${CHAR_C6}_capital_AE")

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

	# ---------------------------------------------------------------------------

	INPUT=$(printf "${UTF8_00DE} capital thorn")
	OUTPUT=$(printf "${UTF8_00DE}_capital_thorn")

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

	# ---------------------------------------------------------------------------

	INPUT=$(printf "${CHAR_DE} capital thorn")
	OUTPUT=$(printf "${CHAR_DE}_capital_thorn")

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

	# ---------------------------------------------------------------------------

	INPUT=$(printf "${UTF8_0172} capital U with Ogonek")
	OUTPUT=$(printf "${UTF8_0172}_capital_U_with_Ogonek")

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

	# ---------------------------------------------------------------------------

	INPUT=$(printf "${CHAR_7E} tilde")
	OUTPUT="~_tilde"

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

	# ---------------------------------------------------------------------------

	INPUT=$(printf "${CHAR_7F} delete")
	OUTPUT="delete"

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

done

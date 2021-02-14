#!/bin/bash -e
#
# Confirms that GitHub issue #29 is fixed
#
# https://github.com/dharple/detox/issues/29
#

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. $TESTBASE/test-functions

DETOX=$1
TABLEPATH=$(dirname $TESTBASE)/table
METHOD2=wipeup

for METHOD1 in safe safe-basic ; do

	# ---------------------------------------------------------------------------

	INPUT="  safe string"
	OUTPUT="safe_string"

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

	# ---------------------------------------------------------------------------

	INPUT=$(printf "\\x07 bell")
	OUTPUT="bell"

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

	# ---------------------------------------------------------------------------

	INPUT=$(printf "\\x09 tab \\x0a new line")
	OUTPUT="tab_new_line"

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

	# ---------------------------------------------------------------------------

	INPUT=$(printf "\\u00C6 capital AE")
	OUTPUT=$(printf "\\u00C6_capital_AE")

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

	# ---------------------------------------------------------------------------

	INPUT=$(printf "\\xC6 capital AE")
	OUTPUT=$(printf "\\xC6_capital_AE")

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

	# ---------------------------------------------------------------------------

	INPUT=$(printf "\\u00DE capital thorn")
	OUTPUT=$(printf "\\u00DE_capital_thorn")

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

	# ---------------------------------------------------------------------------

	INPUT=$(printf "\\xDE capital thorn")
	OUTPUT=$(printf "\\xDE_capital_thorn")

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

	# ---------------------------------------------------------------------------

	INPUT=$(printf "\\u0172 capital U with Ogonek")
	OUTPUT=$(printf "\\u0172_capital_U_with_Ogonek")

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

	# ---------------------------------------------------------------------------

	INPUT=$(printf "\\x7e tilde")
	OUTPUT="~_tilde"

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

	# ---------------------------------------------------------------------------

	INPUT=$(printf "\\x7f delete")
	OUTPUT="delete"

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

done

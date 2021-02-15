#!/bin/bash -e
#
# Confirms that GitHub issue #40 is fixed
#
# https://github.com/dharple/detox/issues/40
#

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. $TESTBASE/test-functions

DETOX=$1
TABLEPATH=$(dirname $TESTBASE)/table

for METHOD1 in utf_8 utf_8-basic ; do

	# ---------------------------------------------------------------------------

	INPUT="_ () whatever"
	OUTPUT=$INPUT

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

	# ---------------------------------------------------------------------------

	INPUT=$(printf "\\u4000 ignore")
	OUTPUT="_ ignore"

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

	# ---------------------------------------------------------------------------

	INPUT=$(printf "\\u007F delete")
	OUTPUT="_ delete"

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

	# ---------------------------------------------------------------------------

	# 2 byte UTF-8 encoded \x00 (null)

	INPUT=$(printf "\\xc0\\x80 null")
	OUTPUT="_hidden_null_ null"

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

	# ---------------------------------------------------------------------------

	# 2 byte UTF-8 encoded \x20 (space)

	INPUT=$(printf "\\xc0\\xa0 space")
	OUTPUT="  space"

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

	# ---------------------------------------------------------------------------

	# 2 byte UTF-8 encoded \x30 (zero)

	INPUT=$(printf "\\xc0\\xb0 zero")
	OUTPUT="0 zero"

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

	# ---------------------------------------------------------------------------

	# 2 byte UTF-8 encoded \x7e (tilde)

	INPUT=$(printf "\\xc1\\xbe tilde")
	OUTPUT="~ tilde"

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

	# ---------------------------------------------------------------------------

	# 2 byte UTF-8 encoded \x7f (delete)

	INPUT=$(printf "\\xc1\\xbf delete")
	OUTPUT="_ delete"

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

done

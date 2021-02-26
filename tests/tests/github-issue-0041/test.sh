#!/usr/bin/env -S bash -e
#
# Confirms that GitHub issue #41 is fixed
#
# https://github.com/dharple/detox/issues/41
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

	INPUT="safe"
	OUTPUT=$INPUT

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

	# ---------------------------------------------------------------------------

	INPUT=$(printf "${UTF8_4000}unknown")
	OUTPUT="_unknown"

	test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

done

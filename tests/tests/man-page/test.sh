#!/bin/bash
#
# Run legacy table regression testing
#

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. $TESTBASE/test-functions

DETOX=$1
INLINE=$(dirname $DETOX)/inline-detox

# ----------------------------------------------------------

INPUT=$(printf "\t \$ &")

# ----------------------------------------------------------

OUTPUT="tab_dollar_and_"
LANG=C.utf-8

CHECK=$(echo "$INPUT" | $INLINE -f detoxrc.manpage)
if [ "$CHECK" != "$OUTPUT" ] ; then
	echo "Man page is a lie"
	exit 1
fi

# ----------------------------------------------------------

OUTPUT="tab_dollar_and_"
LANG=de_DE.utf-8

CHECK=$(echo "$INPUT" | $INLINE -f detoxrc.manpage)
if [ "$CHECK" != "$OUTPUT" ] ; then
	echo "Man page is a lie"
	exit 1
fi

# ----------------------------------------------------------

OUTPUT="tab_money_and_"
LANG=en_US.utf-8

CHECK=$(echo "$INPUT" | $INLINE -f detoxrc.manpage)
if [ "$CHECK" != "$OUTPUT" ] ; then
	echo "Man page is a lie"
	exit 1
fi

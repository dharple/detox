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

locale -a

# ----------------------------------------------------------

INPUT=$(printf "\t \$ &")

# ----------------------------------------------------------

OUTPUT="tab_dollar_and_"
LANG=C.UTF-8
LC_ALL=$LANG
LC_CTYPE=$LANG

CHECK=$(echo "$INPUT" | $INLINE -f detoxrc.manpage)
if [ "$CHECK" != "$OUTPUT" ] ; then
	echo "Man page is a lie.  LANG=$LANG"
	locale
	exit 1
fi

# ----------------------------------------------------------

OUTPUT="tab_dollar_and_"
LANG=de_DE.UTF-8
LC_ALL=$LANG
LC_CTYPE=$LANG

CHECK=$(echo "$INPUT" | $INLINE -f detoxrc.manpage)
if [ "$CHECK" != "$OUTPUT" ] ; then
	echo "Man page is a lie.  LANG=$LANG"
	locale
	exit 1
fi

# ----------------------------------------------------------

OUTPUT="tab_money_and_"
LANG=en_US.UTF-8
LC_ALL=$LANG
LC_CTYPE=$LANG

CHECK=$(echo "$INPUT" | $INLINE -f detoxrc.manpage)
if [ "$CHECK" != "$OUTPUT" ] ; then
	echo "Man page is a lie.  LANG=$LANG"
	locale
	exit 1
fi

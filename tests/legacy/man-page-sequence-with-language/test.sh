#!/usr/bin/env bash
#
# Confirm that the language selection from the detox.tbl.5 man page works as
# described.
#

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. "$TESTBASE"/test-functions.sh
. "$TESTBASE"/character-helper.sh

DETOX=$1
INLINE=$(dirname "$DETOX")/inline-detox

# ----------------------------------------------------------

INPUT=$(printf "\t \$ &")

# ----------------------------------------------------------

WHICH=C.UTF-8

FOUND=$(locale -a | grep -c '^C[.]')

if [ "$FOUND" -gt 0 ] ; then
	OUTPUT="tab_dollar_and_"
	for VAR in LANG LC_ALL LC_CTYPE ; do
		eval "$VAR=$WHICH"
		export $VAR
	done

	CHECK=$(echo "$INPUT" | $INLINE -f detoxrc.manpage)
	if [ "$CHECK" != "$OUTPUT" ] ; then
		echo "Man page is a lie. LANG=$WHICH"
		locale
		exit 1
	fi
else
	echo "skipping LANG=$WHICH ... not installed"
fi

# ----------------------------------------------------------

WHICH=de_DE.UTF-8

FOUND=$(locale -a | grep -c '^de_DE')

if [ "$FOUND" -gt 0 ] ; then
	OUTPUT="tab_dollar_and_"
	for VAR in LANG LC_ALL LC_CTYPE ; do
		eval "$VAR=$WHICH"
		export $VAR
	done

	CHECK=$(echo "$INPUT" | $INLINE -f detoxrc.manpage)
	if [ "$CHECK" != "$OUTPUT" ] ; then
		echo "Man page is a lie. LANG=$WHICH"
		locale
		exit 1
	fi
else
	echo "skipping LANG=$WHICH ... not installed"
fi

# ----------------------------------------------------------

WHICH=en_US.UTF-8

FOUND=$(locale -a | grep -c '^en_US')

if [ "$FOUND" -gt 0 ] ; then
	OUTPUT="tab_money_and_"
	for VAR in LANG LC_ALL LC_CTYPE ; do
		eval "$VAR=$WHICH"
		export $VAR
	done

	CHECK=$(echo "$INPUT" | $INLINE -f detoxrc.manpage)
	if [ "$CHECK" != "$OUTPUT" ] ; then
		echo "Man page is a lie. LANG=$WHICH"
		locale
		exit 1
	fi
else
	echo "skipping LANG=$WHICH ... not installed"
fi

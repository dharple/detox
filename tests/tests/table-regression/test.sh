#!/usr/bin/env -S bash
#
# Run legacy table regression testing
#

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. $TESTBASE/test-functions.sh
. $TESTBASE/character-helper.sh

DETOX=$1
REGRESS=$(dirname $DETOX)/test-table-regression

TIMEOUT=$(command -v timeout)
TIMELIMIT=3s

if [ -n "$USE_VALGRIND" ] ; then
	VALGRIND=$(command -v valgrind || true)

	if [ -z "$VALGRIND" ] ; then
		echo "USE_VALGRIND specified, but valgrind is not installed"
		exit 1
	else
		TIMELIMIT=15s
		REGRESS="$VALGRIND --quiet --error-exitcode=1 --track-origins=yes $REGRESS"
	fi
fi

if [ -n "$TIMEOUT" ] ; then
	$TIMEOUT $TIMELIMIT $REGRESS
	OUT=$?
	if [ $OUT -eq 124 ] ; then
		echo "test timed out; re-running verbose"
		$TIMEOUT $TIMELIMIT $REGRESS -v
		exit 1
	fi
else
	echo timeout is not available, running verbose
	$REGRESS -v
fi

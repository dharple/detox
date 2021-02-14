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
REGRESS=$(dirname $DETOX)/test-table-regression

TIMEOUT=$(command -v timeout)
TIMELIMIT=3s

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

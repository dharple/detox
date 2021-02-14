#!/bin/bash -e
#
# Run legacy table regression testing
#

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. $TESTBASE/test-functions

DETOX=$1
TRUE_REGRESS=$(dirname $DETOX)/true-regress

$TRUE_REGRESS

#!/bin/bash -e
#
# Confirm a statement on the detoxrc.5 manpage
#

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. $TESTBASE/test-functions

DETOX=$1

$DETOX -f detoxrc.manpage -L -v | diff - expected.txt

#!/usr/bin/env bash
#
# Tests to confirm GitHub issue #37 is resolved.  Confirms that an invalid
# config file won't cause a segfault.
#
# https://github.com/dharple/detox/issues/37
#

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. "$TESTBASE"/test-functions.sh
. "$TESTBASE"/character-helper.sh

DETOX=$1

# ------------------------------------------------------

CHECK=$($DETOX -L)
if [ $? -ne 0 ] ; then
	echo "detox -L exited with a non-zero status"
	exit 1
fi

# ------------------------------------------------------

CHECK=$($DETOX -f detoxrc.invalid.1 -L 2>&1)
if [ $? -eq 0 ] ; then
	echo "detox -L with an invalid config file exited with a zero status (file 1)"
	exit 1
fi

EXPECTED="detox: error parsing config file detoxrc.invalid.1: syntax error
	line 4: broken"
if [ "$CHECK" != "$EXPECTED" ] ; then
	echo "detox -L with an invalid config file did not return the expected message (file 1)"
	echo "Expected: $EXPECTED"
	echo "Actual: $CHECK"
	exit 1
fi

# ------------------------------------------------------

CHECK=$($DETOX -f detoxrc.invalid.2 -L 2>&1)
if [ $? -eq 0 ] ; then
	echo "detox -L with an invalid config file exited with a zero status (file 2)"
	exit 1
fi

EXPECTED="detox: error parsing config file detoxrc.invalid.2: syntax error
	line 4: broken"
if [ "$CHECK" != "$EXPECTED" ] ; then
	echo "detox -L with an invalid config file did not return the expected message (file 2)"
	echo "Expected: $EXPECTED"
	echo "Actual: $CHECK"
	exit 1
fi

# ------------------------------------------------------

CHECK=$($DETOX -f detoxrc.invalid.3 -L 2>&1)
if [ $? -eq 0 ] ; then
	echo "detox -L with an invalid config file exited with a zero status (file 3)"
	exit 1
fi

EXPECTED="detox: error parsing config file detoxrc.invalid.3: syntax error
	line 4: "
if [ "$CHECK" != "$EXPECTED" ] ; then
	echo "detox -L with an invalid config file did not return the expected message (file 3)"
	echo "Expected: $EXPECTED"
	echo "Actual: $CHECK"
	exit 1
fi

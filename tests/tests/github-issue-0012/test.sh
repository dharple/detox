#!/bin/bash -e
#
# Tests to confirm GitHub issue #12 is resolved
#
# https://github.com/dharple/detox/issues/12
#

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. $TESTBASE/test-functions

DETOX=$1

BASE=/tmp/detoxtest/
if [ ! -d $BASE ] ; then
	mkdir $BASE
fi
WORK=$(realpath $(mktemp -d $BASE/test-custom-XXXXXX))

# -----

cd $WORK
touch "hi there" "another test" "wow"
EXPECTED="hi_there another_test wow"

$DETOX -r .

for OUTPUT in $EXPECTED ; do 
	if [ ! -f "$OUTPUT" ] ; then
		echo expected "$OUTPUT" not found
		exit 1
	fi
done

# -----

cd $WORK
touch "hi there2" "another test2" "wow2"
EXPECTED="hi_there2 another_test2 wow2"

cd $TESTBASE
$DETOX -r $WORK
cd $WORK

for OUTPUT in $EXPECTED ; do 
	if [ ! -f "$OUTPUT" ] ; then
		echo expected "$OUTPUT" not found
		exit 1
	fi
done

# -----

cd $WORK
mkdir 3 confirm.3
cd 3
touch "hi there3" "another test3" "wow3"
EXPECTED="hi_there3 another_test3 wow3"

cd $TESTBASE
$DETOX -r $WORK/confirm.3/..
cd $WORK/3

for OUTPUT in $EXPECTED ; do 
	if [ ! -f "$OUTPUT" ] ; then
		echo expected "$OUTPUT" not found
		exit 1
	fi
done

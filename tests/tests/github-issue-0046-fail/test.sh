#!/usr/bin/env -S bash
#
# Confirms that GitHub issue #46 is fixed
#
# https://github.com/dharple/detox/issues/46
#

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. $TESTBASE/test-functions.sh
. $TESTBASE/character-helper.sh

DETOX=$1

BASE=/tmp/detoxtest/
if [ ! -d $BASE ] ; then
	mkdir $BASE
fi
WORK=$(realpath $(mktemp -d $BASE/test-custom-XXXXXX))

# ----

cp detoxrc.max_length $WORK/detoxrc
cd $WORK

INPUT="safe and stuff.tar.gz"

touch "$INPUT"
$DETOX -f detoxrc "$INPUT"
if [ $? -ne 0 ] ; then
	echo "non-zero exit code"
	exit 1
fi

if [ ! -f "$INPUT" ] ; then
	echo "input file $INPUT was changed"
	exit 1
fi

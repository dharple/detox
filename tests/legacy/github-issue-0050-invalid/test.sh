#!/usr/bin/env bash
#
# Confirms that GitHub issue #50 is fixed
#
# https://github.com/dharple/detox/issues/50
#

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. "$TESTBASE"/test-functions.sh
. "$TESTBASE"/character-helper.sh

DETOX=$1

SEGFAULT=139

BASE=/tmp/detoxtest/
if [ ! -d $BASE ] ; then
	mkdir $BASE
fi
WORK=$(realpath $(mktemp -d $BASE/test-custom-XXXXXX))

# ----

cp detoxrc.invalid_builtin "$WORK"/detoxrc
cd "$WORK" || exit

INPUT="random filename.txt"

touch "$INPUT"
$DETOX -f detoxrc "$INPUT" 2>&1
RET=$?
if [ $RET -eq "$SEGFAULT" ] ; then
	echo "possible segfault"
	exit 1
fi

if [ $RET -ne 1 ] ; then
	echo "detox didn't reject an invalid builtin table name"
	exit 1
fi

if [ ! -f "$INPUT" ] ; then
	echo "input file $INPUT was changed"
	exit 1
fi

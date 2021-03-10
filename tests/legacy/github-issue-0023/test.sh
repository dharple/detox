#!/usr/bin/env bash
#
# Tests to confirm GitHub issue #23 is resolved
#
# https://github.com/dharple/detox/issues/23
#

set -e

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. "$TESTBASE"/test-functions.sh
. "$TESTBASE"/character-helper.sh

DETOX=$1

BASE=/tmp/detoxtest/
if [ ! -d $BASE ] ; then
	mkdir $BASE
fi
WORK=$(realpath $(mktemp -d $BASE/test-custom-XXXXXX))
CHECK=$WORK/artephius.txt

# -------------------------------------------------

# set up symlink loops within the testing directory
# create a single file to work on

cd "$WORK"
mkdir test
cd test
ln -s .. simple_loop
ln -s . local_loop
mkdir deeper-test
cd deeper-test
touch 'hi there'
mkdir further
cd further

cd "$TESTBASE"
$DETOX -n -r --special "$WORK" > "$CHECK" 2>&1
COUNT=$(wc -l "$CHECK" | sed -e's/^ *//g' | cut -f 1 -d ' ')

if [ "$COUNT" -ne "1" ] ; then
	echo expected 1 line of output from detox, found "$COUNT" instead
	exit 1
fi

# -------------------------------------------------

# run again with no changes to confirm that the next test won't hit a false
# positive

$DETOX -n -r --special "$WORK" > "$CHECK" 2>&1
COUNT=$(wc -l "$CHECK" | sed -e's/^ *//g' | cut -f 1 -d ' ')

if [ "$COUNT" -ne "1" ] ; then
	echo expected 1 line of output from detox, found "$COUNT" instead
	exit 1
fi

# -------------------------------------------------

# set up a recursive loop

cd "$WORK"/test/deeper-test/further/
ln -s ../.. long_loop

cd "$TESTBASE"

if [ ! -d "$WORK"/test/deeper-test/further/long_loop/deeper-test/ ] ; then
	echo test design failed
	exit 1
fi

$DETOX -n -r --special "$WORK" > "$CHECK" 2>&1
COUNT=$(wc -l "$CHECK" | sed -e's/^ *//g' | cut -f 1 -d ' ')

if [ "$COUNT" -ne "1" ] ; then
	echo expected 1 line of output from detox, found "$COUNT" instead
	exit 1
fi

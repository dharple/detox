#!/usr/bin/env bash
#
# Tests to confirm GitHub issue #20 is resolved
#
# https://github.com/dharple/detox/issues/20
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

# -------------------------------------------------

WORK=$(realpath $(mktemp -d $BASE/test-custom-XXXXXX))
cd "$WORK"

SOURCE=fool
INPUT="hi there"
OUTPUT=$INPUT

touch "$SOURCE"
ln -s "$SOURCE" "$INPUT"

$DETOX "$INPUT"

if [ ! -f "$OUTPUT" ] ; then
	echo "Expected $OUTPUT is not present"
	exit 1
fi

# --------------------------------------------------

# same test, but with --special

OUTPUT="hi_there"

$DETOX --special "$INPUT"

if [ ! -f "$OUTPUT" ] ; then
	echo "Expected $OUTPUT is not present"
	exit 1
fi

# --------------------------------------------------

# repeat these tests, in a subdirectory

WORK=$(realpath $(mktemp -d $BASE/test-custom-XXXXXX))
cd "$WORK"

mkdir test
cd test

SOURCE=fool
INPUT="test/hi there"
OUTPUT="$INPUT"

touch "$SOURCE"
cd "$WORK"
ln -s "$SOURCE" "$INPUT"

$DETOX -r "$INPUT"

if [ ! -f "$OUTPUT" ] ; then
	echo "Expected $OUTPUT is not present"
	exit 1
fi

# --------------------------------------------------

# same test, but with --special

OUTPUT="test/hi_there"

$DETOX --special -r "$INPUT"

if [ ! -f "$OUTPUT" ] ; then
	echo "Expected $OUTPUT is not present"
	exit 1
fi


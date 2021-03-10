#!/usr/bin/env bash
#
# Tests to confirm GitHub issue #64 is resolved
#
# https://github.com/dharple/detox/issues/64
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

DETOXRC1=$(dirname $(realpath "$0"))/detoxrc.base
DETOXRC2=$(dirname $(realpath "$0"))/detoxrc.remove_trailing

# -------------------------------------------------

WORK=$(realpath $(mktemp -d $BASE/test-custom-XXXXXX))
cd "$WORK"

mkdir test

INPUT="test/.hi there"
OUTPUT="$INPUT"

touch "$INPUT"
cd "$WORK"

$DETOX -r test
$DETOX -f "$DETOXRC1" -r .
$DETOX -f "$DETOXRC2" -r .

if [ ! -f "$OUTPUT" ] ; then
	echo "Expected $OUTPUT is not present"
	exit 1
fi

# -------------------------------------------------

WORK=$(realpath $(mktemp -d $BASE/test-custom-XXXXXX))
cd "$WORK"

mkdir .git

INPUT=".git/.hi there"
OUTPUT="$INPUT"

touch "$INPUT"
cd "$WORK"

$DETOX -r .
$DETOX -f "$DETOXRC1" -r .
$DETOX -f "$DETOXRC2" -r .

if [ ! -f "$OUTPUT" ] ; then
	echo "Expected $OUTPUT is not present"
	exit 1
fi

# -------------------------------------------------

WORK=$(realpath $(mktemp -d $BASE/test-custom-XXXXXX))
cd "$WORK"

mkdir -p .git/objects/

INPUT=".git/objects/hi there"
OUTPUT="$INPUT"

touch "$INPUT"
cd "$WORK"

$DETOX -r .
$DETOX -f "$DETOXRC1" -r .
$DETOX -f "$DETOXRC2" -r .

if [ ! -f "$OUTPUT" ] ; then
	echo "Expected $OUTPUT is not present"
	exit 1
fi

# -------------------------------------------------

WORK=$(realpath $(mktemp -d $BASE/test-custom-XXXXXX))
cd "$WORK"

mkdir -p {arch}/objects/

INPUT="{arch}/objects/hi there"
OUTPUT="$INPUT"

touch "$INPUT"
cd "$WORK"

# {arch} isn't built in to the base config
$DETOX -f "$DETOXRC1" -r .
$DETOX -f "$DETOXRC2" -r .

if [ ! -f "$OUTPUT" ] ; then
	echo "Expected $OUTPUT is not present"
	exit 1
fi

# -------------------------------------------------

WORK=$(realpath $(mktemp -d $BASE/test-custom-XXXXXX))
cd "$WORK"

mkdir -p {arch}/objects/

INPUT="{arch}/objects/hi there"
OUTPUT="$INPUT"

touch "$INPUT"
cd "$WORK/{arch}"

# {arch} isn't built in to the base config
$DETOX -f "$DETOXRC1" -r ..
$DETOX -f "$DETOXRC2" -r ..

cd "$WORK"

if [ ! -f "$OUTPUT" ] ; then
	echo "Expected $OUTPUT is not present"
	exit 1
fi

#!/usr/bin/env bash
#
# Confirms that GitHub issue #46 is fixed
#
# https://github.com/dharple/detox/issues/46
#

set -e

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. $TESTBASE/test-functions.sh
. $TESTBASE/character-helper.sh

DETOX=$1
TABLEPATH=$(dirname $(dirname $TESTBASE))/table

# ---------------------------------------------------------------------------

INPUT="safe.txt"
OUTPUT=$INPUT
METHOD1=max-length-32

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

# ---------------------------------------------------------------------------

INPUT="safe.and.stuff.txt"
OUTPUT="safe.and.s.txt"
METHOD1=max-length-14

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

# ---------------------------------------------------------------------------

INPUT="safe.and.stu.ff.txt"
OUTPUT="safe.an.ff.txt"
METHOD1=max-length-14

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

# ---------------------------------------------------------------------------

INPUT="safe and stuff.txt"
OUTPUT="safe and.txt"
METHOD1=max-length-12

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

# ---------------------------------------------------------------------------

INPUT="safe and stuff.txt"
OUTPUT="safe.txt"
METHOD1=max-length-8

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

# ---------------------------------------------------------------------------

INPUT="safe.tar.gz"
OUTPUT=$INPUT
METHOD1=max-length-32

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

# ---------------------------------------------------------------------------

INPUT="safe and stuff.tar.gz"
OUTPUT="safe .tar.gz"
METHOD1=max-length-12

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

# ---------------------------------------------------------------------------

INPUT="safe and stuff.tar.gz"
OUTPUT="sa.tar.gz"
METHOD1=max-length-9

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

# ---------------------------------------------------------------------------

INPUT="safe and stuff.tar.gz"
OUTPUT="s.tar.gz"
METHOD1=max-length-8

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

# ---------------------------------------------------------------------------

# max-length-7 is in github-issue=0046-fail

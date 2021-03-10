#!/usr/bin/env bash
#
# Tests to confirm GitHub issue #11 is resolved
#
# https://github.com/dharple/detox/issues/11
#

set -e

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. "$TESTBASE"/test-functions.sh
. "$TESTBASE"/character-helper.sh

DETOX=$1
INPUT=$(printf '     '$'\n\n''song name'$'\n''by band'$'\n''/')
OUTPUT="_______song_name_by_band_"
METHOD="safe"
TABLE=$(dirname $(dirname "$TESTBASE"))/table/safe.tbl

test_single_table "$DETOX" "$INPUT" "$OUTPUT" "$METHOD" "$TABLE"

# ---------------------------------------------------------------------------

METHOD1=safe
TABLEPATH=$(dirname $(dirname "$TESTBASE"))/table

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

# ---------------------------------------------------------------------------

METHOD1=safe-basic

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

# ---------------------------------------------------------------------------

OUTPUT="$INPUT"
METHOD1=uncgi

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1"

# ---------------------------------------------------------------------------

OUTPUT="song_name_by_band_"
METHOD1=uncgi
METHOD2=safe
METHOD3=wipeup

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2" "$METHOD3"

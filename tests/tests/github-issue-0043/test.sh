#!/bin/bash -e
#
# Subset of legacy regression tests: remove_trailing
#

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. $TESTBASE/test-functions

DETOX=$1
TABLEPATH=$(dirname $TESTBASE)/table
METHOD1a=wipeup
METHOD1b=wipeup-remove-trailing

# ---------------------------------------------------------------------------

INPUT="hi there"
OUTPUTa="$INPUT"
OUTPUTb="$INPUT"

test_sequence "$DETOX" "$INPUT" "$OUTPUTa" "$TABLEPATH" "$METHOD1a"
test_sequence "$DETOX" "$INPUT" "$OUTPUTb" "$TABLEPATH" "$METHOD1b"

# ---------------------------------------------------------------------------

INPUT="-hi there"
OUTPUTa="hi there"
OUTPUTb="hi there"

test_sequence "$DETOX" "$INPUT" "$OUTPUTa" "$TABLEPATH" "$METHOD1a"
test_sequence "$DETOX" "$INPUT" "$OUTPUTb" "$TABLEPATH" "$METHOD1b"

# ---------------------------------------------------------------------------

INPUT="_hi there"
OUTPUTa="hi there"
OUTPUTb="hi there"

test_sequence "$DETOX" "$INPUT" "$OUTPUTa" "$TABLEPATH" "$METHOD1a"
test_sequence "$DETOX" "$INPUT" "$OUTPUTb" "$TABLEPATH" "$METHOD1b"

# ---------------------------------------------------------------------------

INPUT="#hi there"
OUTPUTa="hi there"
OUTPUTb="hi there"

test_sequence "$DETOX" "$INPUT" "$OUTPUTa" "$TABLEPATH" "$METHOD1a"
test_sequence "$DETOX" "$INPUT" "$OUTPUTb" "$TABLEPATH" "$METHOD1b"

# ---------------------------------------------------------------------------

INPUT="-_#--hi there"
OUTPUTa="hi there"
OUTPUTb="hi there"

test_sequence "$DETOX" "$INPUT" "$OUTPUTa" "$TABLEPATH" "$METHOD1a"
test_sequence "$DETOX" "$INPUT" "$OUTPUTb" "$TABLEPATH" "$METHOD1b"

# ---------------------------------------------------------------------------

INPUT="hi-there"
OUTPUTa="hi-there"
OUTPUTb="hi-there"

test_sequence "$DETOX" "$INPUT" "$OUTPUTa" "$TABLEPATH" "$METHOD1a"
test_sequence "$DETOX" "$INPUT" "$OUTPUTb" "$TABLEPATH" "$METHOD1b"

# ---------------------------------------------------------------------------

INPUT="hi---there"
OUTPUTa="hi-there"
OUTPUTb="hi-there"

test_sequence "$DETOX" "$INPUT" "$OUTPUTa" "$TABLEPATH" "$METHOD1a"
test_sequence "$DETOX" "$INPUT" "$OUTPUTb" "$TABLEPATH" "$METHOD1b"

# ---------------------------------------------------------------------------

INPUT="hi.-there"
OUTPUTa="hi.-there"
OUTPUTb="hi.there"

test_sequence "$DETOX" "$INPUT" "$OUTPUTa" "$TABLEPATH" "$METHOD1a"
test_sequence "$DETOX" "$INPUT" "$OUTPUTb" "$TABLEPATH" "$METHOD1b"

# ---------------------------------------------------------------------------

INPUT="hi.-.-there"
OUTPUTa="hi.-.-there"
OUTPUTb="hi.there"

test_sequence "$DETOX" "$INPUT" "$OUTPUTa" "$TABLEPATH" "$METHOD1a"
test_sequence "$DETOX" "$INPUT" "$OUTPUTb" "$TABLEPATH" "$METHOD1b"

# ---------------------------------------------------------------------------

INPUT="hi_-_-there"
OUTPUTa="hi-there"
OUTPUTb="hi-there"

test_sequence "$DETOX" "$INPUT" "$OUTPUTa" "$TABLEPATH" "$METHOD1a"
test_sequence "$DETOX" "$INPUT" "$OUTPUTb" "$TABLEPATH" "$METHOD1b"

# ---------------------------------------------------------------------------

INPUT="hi_-_-there...-.--"
OUTPUTa="hi-there...-.-"
OUTPUTb="hi-there."

test_sequence "$DETOX" "$INPUT" "$OUTPUTa" "$TABLEPATH" "$METHOD1a"
test_sequence "$DETOX" "$INPUT" "$OUTPUTb" "$TABLEPATH" "$METHOD1b"

# ---------------------------------------------------------------------------

INPUT="hi_-_-there...-.--_-_-________----_-____-test"
OUTPUTa="hi-there...-.-test"
OUTPUTb="hi-there.test"

test_sequence "$DETOX" "$INPUT" "$OUTPUTa" "$TABLEPATH" "$METHOD1a"
test_sequence "$DETOX" "$INPUT" "$OUTPUTb" "$TABLEPATH" "$METHOD1b"

# ---------------------------------------------------------------------------

INPUT="hi there-"
OUTPUTa="hi there-"
OUTPUTb="hi there-"

test_sequence "$DETOX" "$INPUT" "$OUTPUTa" "$TABLEPATH" "$METHOD1a"
test_sequence "$DETOX" "$INPUT" "$OUTPUTb" "$TABLEPATH" "$METHOD1b"

# ---------------------------------------------------------------------------

INPUT="hi there_"
OUTPUTa="hi there_"
OUTPUTb="hi there_"

test_sequence "$DETOX" "$INPUT" "$OUTPUTa" "$TABLEPATH" "$METHOD1a"
test_sequence "$DETOX" "$INPUT" "$OUTPUTb" "$TABLEPATH" "$METHOD1b"

# ---------------------------------------------------------------------------

INPUT="hi there."
OUTPUTa="hi there."
OUTPUTb="hi there."

test_sequence "$DETOX" "$INPUT" "$OUTPUTa" "$TABLEPATH" "$METHOD1a"
test_sequence "$DETOX" "$INPUT" "$OUTPUTb" "$TABLEPATH" "$METHOD1b"


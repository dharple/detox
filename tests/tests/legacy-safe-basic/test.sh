#!/usr/bin/env bash
#
# Subset of legacy regression tests: safe
#

set -e

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. $TESTBASE/test-functions

DETOX=$1
TABLEPATH=$(dirname $TESTBASE)/table
METHOD1=safe-basic
METHOD2=wipeup

# ---------------------------------------------------------------------------

INPUT="lower"
OUTPUT="lower"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT="^acute"
OUTPUT="^acute"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT="&ampersand"
OUTPUT="and_ampersand"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT="<angle bracket left"
OUTPUT="angle_bracket_left"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT=">angle bracket right"
OUTPUT="angle_bracket_right"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT="\\back slash"
OUTPUT="back_slash"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT="\`back tick"
OUTPUT="back_tick"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT="|bar"
OUTPUT="bar"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT="{brace left"
OUTPUT="brace_left"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT="}brace right"
OUTPUT="brace_right"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT=":colon"
OUTPUT="colon"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT=",comma"
OUTPUT=",comma"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT="\@commercial at"
OUTPUT="commercial_at"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT="dash-"
OUTPUT="dash-"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT="#dash_octothorpe-"
OUTPUT="dash_octothorpe-"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT="_dash_underscore-"
OUTPUT="dash_underscore-"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT="\$dollar sign"
OUTPUT="dollar_sign"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT="\"double quote"
OUTPUT="double_quote"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT="ends with octothorpe#"
OUTPUT="ends_with_octothorpe#"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT="!exclamation point"
OUTPUT="exclamation_point"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT="#octothorpe"
OUTPUT="octothorpe"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT="#-octothorpe_dash"
OUTPUT="octothorpe_dash"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT="(parenthesis left"
OUTPUT="parenthesis_left"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT=")parenthesis right"
OUTPUT="parenthesis_right"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT="%percent"
OUTPUT="%percent"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT="+plus"
OUTPUT="+plus"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT="?question mark"
OUTPUT="question_mark"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT=";semi colon"
OUTPUT="semi_colon"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT="'single quote"
OUTPUT="single_quote"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT=" space"
OUTPUT="space"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT="[square bracket left"
OUTPUT="square_bracket_left"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT="]square bracket right"
OUTPUT="square_bracket_right"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT="_-underscore_dash"
OUTPUT="underscore_dash"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT="_underscore"
OUTPUT="underscore"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"

# ---------------------------------------------------------------------------

INPUT="~tilde"
OUTPUT="~tilde"

test_sequence "$DETOX" "$INPUT" "$OUTPUT" "$TABLEPATH" "$METHOD1" "$METHOD2"


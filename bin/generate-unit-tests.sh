#!/usr/bin/env bash
#
# Generate tests/unit/*.c from .ts
#

PROJECT_ROOT=$(dirname $(dirname $(realpath $0)))
cd $PROJECT_ROOT

TESTDIR=$PROJECT_ROOT/tests/unit
cd $TESTDIR

CHECKMK=$(command -v checkmk)

if [ ! -x "$CHECKMK" ] ; then
	echo "Please install checkmk, possibly through the 'check' package, before using this"
	exit 1
fi

for FILE in *.ts ; do
	INPUT=$(basename $FILE)
	OUTPUT="${INPUT%.ts}.c"
	echo "process check file $INPUT"
	$CHECKMK $INPUT > $OUTPUT
done

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

BASE=/tmp/detoxtest/
if [ ! -d $BASE ] ; then
	mkdir $BASE
fi
WORK=$(realpath $(mktemp -d $BASE/work-XXXXXX))

for FILE in *.ts ; do
	INPUT=$(basename $FILE)
	OUTPUT="${INPUT%.ts}.c"
	echo -n "process check file $INPUT... "
	$CHECKMK $INPUT > $WORK/$OUTPUT
	diff -q $WORK/$OUTPUT $OUTPUT 2>&1 > /dev/null
	if [ "$?" -eq "0" ] ; then
		echo
		rm $WORK/$OUTPUT
	else
		echo "updated"
		mv $WORK/$OUTPUT $OUTPUT
	fi
done

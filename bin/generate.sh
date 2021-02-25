#!/usr/bin/env -S bash
#
# Generate internals
#

PROJECT_ROOT=$(dirname $(dirname $0))

cd $PROJECT_ROOT

#
# Generate src/builtin_table.c
#

SRCDIR=$PROJECT_ROOT/src
GENERATE=$SRCDIR/generate-builtin-table

if [ ! -x $GENERATE ] ; then
	echo please build $GENERATE first
	exit 1
fi

cp $SRCDIR/builtin_table.c.in $SRCDIR/builtin_table.c

for TABLE in safe iso8859_1 unicode cp1252 ; do
	echo process builtin $TABLE
	$GENERATE $PROJECT_ROOT/table/$TABLE.tbl | sed -e"s/NEW/$TABLE/" >> $SRCDIR/builtin_table.c
done

#
# Generate man/*.pdf
#

MANDIR=$PROJECT_ROOT/man
MANDOC=$(command -v mandoc)

for FILE in $MANDIR/*.[15] ; do
	echo process man page $(basename $FILE)
	$MANDOC -T pdf $FILE > $FILE.pdf
done

#
# Update tests based on man pages
#

TESTDIR=$PROJECT_ROOT/tests

echo process detoxrc.5 into regression test
sed -n '/START SAMPLE/,/END SAMPLE/p' $MANDIR/detoxrc.5 | sed -e's/^[.]." /# /' > $TESTDIR/tests/man-page-example/detoxrc.detoxrc.5

echo process detox.tbl.5 into regression test
sed -n '/START SAMPLE/,/END SAMPLE/p' $MANDIR/detox.tbl.5 | sed -e's/^[.]." /# /' > $TESTDIR/tests/man-page-example/detoxrc.detox.tbl.5

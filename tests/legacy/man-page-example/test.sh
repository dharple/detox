#!/usr/bin/env bash
#
# Confirm that the sample detoxrcs in detoxrc.5 and detox.tbl.5 parse
# correctly.
#

set -e

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. "$TESTBASE"/test-functions.sh
. "$TESTBASE"/character-helper.sh

DETOX=$1

MANPATH=$(dirname $(dirname "$TESTBASE"))/man

sed -n '/START SAMPLE/,/END SAMPLE/p' "$MANPATH"/detoxrc.5 | sed -e's/^[.]." /# /' | diff - detoxrc.detoxrc.5
sed -n '/START SAMPLE/,/END SAMPLE/p' "$MANPATH"/detox.tbl.5 | sed -e's/^[.]." /# /' | diff - detoxrc.detox.tbl.5

$DETOX -f detoxrc.detoxrc.5 -L -v | diff - expected.detoxrc.5.txt
$DETOX -f detoxrc.detox.tbl.5 -L -v | diff - expected.detox.tbl.5.txt

#!/usr/bin/env bash
#
# Generate legacy test fixtures from man pages
#

PROJECT_ROOT=$(dirname "$(dirname "$(realpath "$0")")")
cd "$PROJECT_ROOT" || exit

MANDIR=$PROJECT_ROOT/man
TESTDIR=$PROJECT_ROOT/tests/legacy

echo "process detoxrc.5 into regression test"
sed -n '/START SAMPLE/,/END SAMPLE/p' "$MANDIR"/detoxrc.5 | sed -e's/^[.]." /# /' > "$TESTDIR"/man-page-example/detoxrc.detoxrc.5

echo "process detox.tbl.5 into regression test"
sed -n '/START SAMPLE/,/END SAMPLE/p' "$MANDIR"/detox.tbl.5 | sed -e's/^[.]." /# /' > "$TESTDIR"/man-page-example/detoxrc.detox.tbl.5

#!/usr/bin/env bash
#
# Generate PDFs from man pages
#

PROJECT_ROOT=$(dirname "$(dirname "$(realpath "$0")")")
cd "$PROJECT_ROOT" || exit

MANDIR=$PROJECT_ROOT/man
MANDOC=$(command -v mandoc)

if [ ! -x "$MANDOC" ] ; then
	echo "Please install mandoc"
	exit 1
fi

for FILE in "$MANDIR"/*.[15] ; do
	echo "process man page $(basename "$FILE")"
	$MANDOC -T pdf "$FILE" > "$FILE".pdf
done

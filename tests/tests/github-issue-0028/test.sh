#!/bin/bash -e
#
# Tests to confirm GitHub issue #28 is resolved
#
# https://github.com/dharple/detox/issues/28
#

if [ -z "$TESTBASE" ] ; then
	echo TESTBASE needs to be passed via the env
	exit 1
fi

. $TESTBASE/test-functions

DETOX=$1

TABLEPATH=$(dirname $TESTBASE)/table

# ------------------------------------------------------------------------

BASE=/tmp/detoxtest/
if [ ! -d $BASE ] ; then
	mkdir $BASE
fi

HOLD=$(realpath $(mktemp -d $BASE/test-custom-XXXXXX))

# ------------------------------------------------------------------------

STRACE=$(command -v strace || true)

if [ -z "$STRACE" ] ; then
	echo WARNING: strace is not installed
	exit 0
fi

# ---------------------------------------------------------------------------

INPUT="hi there.txt"
OUTPUT="hi_there.txt"

for METHOD1 in safe safe-basic safe-search ; do
	CHECK="$HOLD/strace.$METHOD1.txt"
	SCRIPT="$HOLD/check.$METHOD1.sh"
	cat <<- DONE >> $SCRIPT
		#!/bin/bash
		. $TESTBASE/test-functions
		DETOX="$DETOX"
		TABLEPATH="$TABLEPATH"
		INPUT="$INPUT"
		OUTPUT="$OUTPUT"
		METHOD1="$METHOD1"
		test_sequence "\$DETOX" "\$INPUT" "\$OUTPUT" "\$TABLEPATH" "\$METHOD1"
	DONE
	$STRACE -f -o "$CHECK" -s 1024 -e trace=file /bin/bash $SCRIPT

	case "$METHOD1" in
		safe | safe-basic )
			COUNT=$(grep -c 'usr.share.detox.safe.tbl' $CHECK || true)
			if [ $COUNT -gt 0 ] ; then
				echo "Found evidence of searching for safe.tbl when using $METHOD1"
				exit 1
			fi
			COUNT=$(grep -c 'usr.local.share.detox.safe.tbl' $CHECK || true)
			if [ $COUNT -gt 0 ] ; then
				echo "Found evidence of searching for safe.tbl when using $METHOD1"
				exit 1
			fi
			;;
		safe-search )
			COUNT=$(grep -c 'usr.share.detox.safe.tbl' $CHECK || true)
			if [ $COUNT -eq 0 ] ; then
				echo "Found NO evidence of searching for safe.tbl when using $METHOD1"
				exit 1
			fi
			COUNT=$(grep -c 'usr.local.share.detox.safe.tbl' $CHECK || true)
			if [ $COUNT -eq 0 ] ; then
				echo "Found NO evidence of searching for safe.tbl when using $METHOD1"
				exit 1
			fi
			;;
	esac

done

#!/bin/bash -x
#
# Wrapper around mallocfail
#
# This is still experimental and most likely will not work on your system.
#

ROOT=$(dirname "$(dirname "$(realpath "$0")")")
SLEEP=0.5s

# --------------------------------------------------------
# onboard mallocfail
# --------------------------------------------------------

MALLOCFAIL=/tmp/detoxtest/mallocfail/
MALLOCFAILSO=$MALLOCFAIL/mallocfail.so

if [ ! -d "$MALLOCFAIL" ] ; then
	cd /tmp
	mkdir -p detoxtest
	cd detoxtest
	git clone https://github.com/ralight/mallocfail.git
	cd mallocfail
	make
else
	if [ ! -f "$MALLOCFAILSO" ] ; then
		cd "$MALLOCFAIL"
		make clean && make
	fi
fi

if [ ! -f "$MALLOCFAILSO" ] ; then
	echo "could not find $MALLOCFAILSO"
	exit 1
fi

cd $ROOT

# --------------------------------------------------------

BASE="/tmp/detoxtest/$(date +"%Y%m%d")"
if [ ! -d $BASE ] ; then
	mkdir -p $BASE
fi
WORK=$(realpath $(mktemp -d $BASE/test-mallocfail-XXXXXX))
FAIL=$WORK/fail
mkdir -p $FAIL

MALLOCFAIL_FILE=$WORK/hashes.txt
export MALLOCFAIL_FILE

MALLOCFAIL_DEBUG=1
export MALLOCFAIL_DEBUG

MALLOCFAIL_FAIL_COUNT=1
export MALLOCFAIL_FAIL_COUNT

COUNT=0

while [ 1 ] ; do
	OUTPUT="$WORK/pass-$(printf "%04d" $COUNT).txt"

	echo "iteration $COUNT"
	LD_PRELOAD=$MALLOCFAILSO $ROOT/src/detox -L -v > $OUTPUT
	EXIT=$?

	if [ "$EXIT" -eq "139" ] ; then
		cat $OUTPUT
		echo "segfault"
		cp $OUTPUT $FAIL/
		sleep $SLEEP
	fi

	if [ "$EXIT" -eq "0" ] ; then
		cat $OUTPUT
		if [ "$(grep -ci "Start trace" $OUTPUT)" -gt "0" ] ; then
			echo "malloc failed but the script didn't exit"
			if [ "$(grep -ci "stdio2/printf" $OUTPUT)" -gt "0" ] ; then
				echo "printf caused this one..."
			fi
			cp $OUTPUT $FAIL/
			sleep $SLEEP
		else
			echo "all tests passed"
			break
		fi
	fi

	COUNT=$(($COUNT+1))
done

ls -al $FAIL

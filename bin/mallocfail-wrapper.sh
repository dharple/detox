#!/bin/bash -x
#
# Wrapper around mallocfail
#
# This is still experimental and most likely will not work on your system.
#

PROJECT_ROOT=$(dirname "$(dirname "$(realpath "$0")")")
SLEEP=0.5s

DETOX="$PROJECT_ROOT/src/detox"
if [ ! -f "$DETOX" ] ; then
	cd "$PROJECT_ROOT" || exit
	make
fi

# --------------------------------------------------------
# onboard mallocfail
# --------------------------------------------------------

MALLOCFAIL=/tmp/detoxtest/mallocfail/
MALLOCFAILSO=$MALLOCFAIL/mallocfail.so

if [ ! -d "$MALLOCFAIL" ] ; then
	cd /tmp || exit
	mkdir -p detoxtest
	cd detoxtest || exit
	git clone https://github.com/ralight/mallocfail.git
	cd mallocfail || exit
	make
else
	if [ ! -f "$MALLOCFAILSO" ] ; then
		cd "$MALLOCFAIL" || exit
		make clean && make
	fi
fi

if [ ! -f "$MALLOCFAILSO" ] ; then
	echo "could not find $MALLOCFAILSO"
	exit 1
fi

cd "$PROJECT_ROOT" || exit

# --------------------------------------------------------

BASE="/tmp/detoxtest/$(date +"%Y%m%d")"
if [ ! -d "$BASE" ] ; then
	mkdir -p "$BASE"
fi
WORK=$(realpath "$(mktemp -d "$BASE"/test-mallocfail-XXXXXX)")
FAIL=$WORK/fail
mkdir -p "$FAIL"

MALLOCFAIL_FILE=$WORK/hashes.txt
export MALLOCFAIL_FILE

MALLOCFAIL_DEBUG=1
export MALLOCFAIL_DEBUG

MALLOCFAIL_FAIL_COUNT=1
export MALLOCFAIL_FAIL_COUNT

COUNT=0

while true ; do
	OUTPUT="$WORK/pass-$(printf "%04d" $COUNT).txt"

	echo "iteration $COUNT"
	# LD_PRELOAD=$MALLOCFAILSO $DETOX -L -v > $OUTPUT
	# LD_PRELOAD=$MALLOCFAILSO $DETOX -f "$PROJECT_ROOT"/etc/detoxrc -L -v > $OUTPUT
	# dmesg | LD_PRELOAD=$MALLOCFAILSO $DETOX -s utf_8-legacy -f "$PROJECT_ROOT"/etc/detoxrc --inline > $OUTPUT
	LD_PRELOAD=$MALLOCFAILSO $DETOX -s utf_8-legacy -f "$PROJECT_ROOT"/etc/detoxrc --dry-run --recursive /tmp > "$OUTPUT"
	EXIT=$?

	if [ "$EXIT" -eq "139" ] ; then
		cat "$OUTPUT"
		echo "segfault"
		cp "$OUTPUT" "$FAIL"/
		sleep $SLEEP
	fi

	if [ "$EXIT" -eq "0" ] ; then
		cat "$OUTPUT"
		if [ "$(grep -ci "Start trace" "$OUTPUT")" -gt "0" ] ; then
			echo "malloc failed but the script didn't exit"
			if [ "$(grep -ci "stdio2/printf" "$OUTPUT")" -gt "0" ] ; then
				echo "printf caused this one..."
			fi
			cp "$OUTPUT" "$FAIL"/
			sleep $SLEEP
		else
			echo "all tests passed"
			break
		fi
	fi

	COUNT=$((COUNT+1))
done

ls -al "$FAIL"

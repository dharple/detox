#!/usr/bin/env bash
#
# Executes all unit tests
#

if [ -z $1 ] ; then
	echo missing detox path
	exit 1
fi

. $(dirname "$0")/test-functions

DETOX=$(realpath $1)
if [ ! -x $DETOX ] ; then
	echo cannot find detox at $1
	exit 1
fi

cd $(dirname $0)

export TESTBASE=$(pwd)

SUCCESS=1

for DIR in tests/* ; do
	echo "------------------------------------------------------"
	echo running test $(basename $DIR)
	cd $DIR
	./test.sh $DETOX
	if [ $? -ne 0 ] ; then
		SUCCESS=0
		echo FAILED
	fi
	cd $TESTBASE
done

echo "------------------------------------------------------"
if [ $SUCCESS -ne 1 ] ; then
	echo one or more tests failed
	exit 1
fi

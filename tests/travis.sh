#!/bin/sh

TESTDIR=$(dirname $0)

if [ "$TRAVIS_OS_NAME" = "osx" ] ; then
	BASH=/usr/local/bin/bash
	if [ ! -x $BASH ] ; then
		echo Could not find updated copy of bash
		exit 1
	fi

	$BASH $TESTDIR/test.sh $@
else 
	$TESTDIR/test.sh $@
fi

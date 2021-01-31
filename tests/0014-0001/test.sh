#!/bin/bash -e

INPUT="mÉ Æ.txt"
OUTPUT="mÉ_Æ.txt"
TABLE="test.tbl"

if [ -z $1 ] ; then
	echo missing detox path
	exit 1
fi

WORK=/tmp/detoxtest/$(basename `pwd`)-$RANDOM/

mkdir -p $WORK

cp $TABLE $WORK

cat << DONE > $WORK/detoxrc
sequence gnu {
	utf_8 {
		filename "$WORK/test.tbl";
	};
};
DONE

cd $WORK

touch "mÉ Æ.txt"
$1 -v -s gnu -f $WORK/detoxrc *txt
if [ ! -f $OUTPUT ] ; then
	echo expected output not found
	exit 1
fi

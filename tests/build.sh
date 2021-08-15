#!/usr/bin/env bash

set -ex

# set up working dir

WORK="/tmp/detox-build-test-$RANDOM"
mkdir $WORK
cd $WORK

# git

git clone -b 1.x https://github.com/dharple/detox
cd detox

# configure

autoreconf --install
./configure

# make

make

# tests

src/detox -V
tests/test.sh src/detox

# distcheck

make distcheck

# cleanup

cd
rm -rf $WORK

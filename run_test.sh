#!/usr/bin/env sh


LIBDIR=./build/lib*
PYTHON=`which python3`

$PYTHON setup.py build
cp test_mount.py $LIBDIR
cd $LIBDIR
$PYTHON test_mount.py
cd -
rm -r build

#!/bin/bash

export B_PATH="`pwd`/build"

cd ./test
./test-c.sh
cd ..

LAST=`pwd`

cd /tmp
rm *.o
rm *.asm
rm -r ./ctest
cd $LAST

echo "All testing done."

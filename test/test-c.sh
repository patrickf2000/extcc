#!/bin/bash

echo "Testing C..."
echo ""

if [ ! -d /tmp/ctest ] ; then
	mkdir -p /tmp/ctest
fi

for entry in "./clang"/*.c
do
	entry2=`basename $entry`
	echo "$entry2"
	
	gcc $entry -o /tmp/ctest/$entry2
	expected=`/tmp/ctest/$entry2`

	$B_PATH/cc $entry -o $B_PATH/out.bin
	actual=`$B_PATH/out.bin`

	if [[ $expected == $actual ]] ;then
		echo "Pass"
	else
		echo "Fail"
		echo ""
		echo "Expected: $expected"
		echo "Actual: $actual"
		exit 1
	fi
	
	echo ""
done

echo "Done"

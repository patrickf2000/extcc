#!/bin/bash

function run_test() {
	entry=$1
	
	entry2=`basename $entry`
	echo "$entry2"
	
	gcc $entry -o /tmp/ctest/$entry2
	$B_PATH/cc $entry -o $B_PATH/out.bin
	
	diff <(/tmp/ctest/$entry2) <($B_PATH/out.bin)

	if [[ $? == 0 ]] ;then
		/tmp/ctest/$entry2 > /dev/null
		ret1=$?
		
		$B_PATH/out.bin > /dev/null
		ret2=$?
		
		if [[ $ret1 == $ret2 ]] ; then
			echo "Pass"
		else
			echo "Fail on return values:"
			echo "Expected: $ret1"
			echo "Actual: $ret2"
			exit 1
		fi
	else
		echo "Fail"
		echo ""
		echo "Expected: "
		echo "`/tmp/ctest/$entry2`"
		echo "Actual:"
		echo "`$B_PATH/out.bin`"
		exit 1
	fi
	
	echo ""
}

echo "Testing C..."
echo ""

if [ ! -d /tmp/ctest ] ; then
	mkdir -p /tmp/ctest
fi

echo "Running feature tests...."
echo "====================================="
for entry in "./clang/feature"/*.c
do
	run_test $entry
done

echo ""
echo "Running mini-tests..."
echo "====================================="
for entry in "./clang"/*.c
do
	run_test $entry
done

echo "Done"

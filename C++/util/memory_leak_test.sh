#!/bin/bash

if [ ! -f "$1" ]; then
	echo "Usage: bash memory_leak_test.sh path_of_exe"
	exit 1
fi

debug_output=$(valgrind --leak-check=full -q $1 2>&1 | grep "^=")
if [[ ! -z $debug_output ]]; then
	echo "$debug_output"
	echo "Valgrind failed!"
	exit 1
fi

echo "Valgrind passed!"
exit 0

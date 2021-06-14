#!/bin/bash

if [ -z "$1" ]; then
	echo "Usage: bash memory_leak_test.sh path_of_exe"
	exit 1
fi

if [[ ! -z $(valgrind --leak-check=full -q $1 2>&1 | grep "^=") ]]; then
	echo "Valgrind failed!"
	exit 1
fi

echo "Valgrind passed!"
exit 0

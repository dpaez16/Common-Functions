#!/bin/bash

[ -d "coverage" ] && rm -rf coverage
mkdir coverage

make gcov
obj_files=$(ls ../ | grep ".\.o")

for filename in $obj_files
do
    base_filename=$(echo $filename | tr -d ".o")
    mv $base_filename.cpp.gcov coverage/.
done

make clean

for filename in $(ls coverage)
do
    mv coverage/$filename .
done

rmdir coverage
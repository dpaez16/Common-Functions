#!/bin/bash

cd ../

for filename in `ls | egrep '\.cpp'`; 
do
    gcov -n -o . $filename > /dev/null; 
done

lcov -c --directory . --output-file cov.info
lcov --remove cov.info -o cov_filtered.info \
    "*/tests/*" \
    "/usr/include/*"
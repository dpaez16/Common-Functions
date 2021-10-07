#!/bin/bash

COV_FILE=cov.info
BIN=bin

cd ../
gcov $BIN/*.o
lcov -c --directory . --output-file $COV_FILE
lcov --remove $COV_FILE -o $COV_FILE \
    "*_test.cpp" \
    "/usr/include/*"

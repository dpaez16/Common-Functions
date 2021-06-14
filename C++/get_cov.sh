#!/bin/bash

make gcov
lcov -c --directory . --output-file cov.info
lcov --remove cov.info -o cov_filtered.info \
    "*_test.cpp" \
    "/usr/include/*"
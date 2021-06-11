#!/bin/bash

make gcov

cd ../

lcov -c --directory . --output-file cov.info
lcov --remove cov.info -o cov_filtered.info \
    "*/tests/*" \
    "/usr/include/*"
#!/bin/bash

make gcov

cd ../

lcov -c --directory . --output-file cov.info
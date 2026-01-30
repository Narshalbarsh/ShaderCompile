#!/usr/bin/env bash

rm -rf build-win64
cmake -S . -B build-win64 -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/mingw64.cmake \
    -DCMAKE_BUILD_TYPE=Release
cmake --build build-win64 --parallel

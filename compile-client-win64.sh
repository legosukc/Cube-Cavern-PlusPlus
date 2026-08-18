#!/bin/bash
mkdir build-win64
cmake -S./ -Bbuild-win64/ -DBUILD_TARGET=client -DCMAKE_TOOLCHAIN_FILE=toolchains/win64-toolchain.cmake
cmake --build build-win64/
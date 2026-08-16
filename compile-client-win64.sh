#!/bin/bash
mkdir build-win64
cmake -DCMAKE_TOOLCHAIN_FILE=toolchains/win64-toolchain.cmake ..
cmake -S./ -Bbuild-win64/ -DBUILD_TARGET=client
cmake --build-win64 build/
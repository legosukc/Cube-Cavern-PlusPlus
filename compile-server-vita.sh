#!/bin/bash
mkdir build-vita
cmake -S./ -Bbuild-vita/ -DBUILD_TARGET=server -DPLATFORM=vita
cmake --build build-vita/
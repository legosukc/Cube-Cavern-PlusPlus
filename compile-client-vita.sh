#!/bin/bash
mkdir build-vita
cmake -S./ -Bbuild-vita/ -DBUILD_TARGET=client -DPLATFORM=vita
cmake --build build-vita/
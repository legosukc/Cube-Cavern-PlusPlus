#!/bin/bash
mkdir build
cmake -S./ -Bbuild/ -DBUILD_TARGET=client
cmake --build build/
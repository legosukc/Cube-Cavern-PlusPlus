#!/bin/bash
mkdir build/
cmake build/ -DBUILD_TARGET=server
cmake --build build/
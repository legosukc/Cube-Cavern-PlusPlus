#!/bin/bash
mkdir build-server/
cmake build-server/ -DBUILD_TARGET=server
cmake --build build-server/
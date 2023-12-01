#!/bin/bash
# chmod +x build_debug.sh          

clear && 
rm -Rf build-debug &&
mkdir -p build-debug &&
cd build-debug &&
conan install .. --build=missing -s build_type=Debug -s compiler.libcxx=libstdc++11 &&
cmake .. -DCMAKE_BUILD_TYPE=Debug &&
cmake --build .
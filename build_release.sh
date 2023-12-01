#!/bin/bash
# chmod +x build_release.sh          

clear && 
rm -Rf build-release &&
mkdir -p build-release &&
cd build-release &&
conan install .. --build=missing -s build_type=Release -s compiler.libcxx=libstdc++11 &&
cmake .. -DCMAKE_BUILD_TYPE=Release &&
cmake --build .
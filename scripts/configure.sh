#!/bin/bash
mkdir -p cmake
cd ./cmake 
rm CMakeCache.txt
if [[ $1 == "-d" ]]; then
    cmake .. -DCMAKE_BUILD_TYPE=Debug
else
    cmake ..
fi
cd ..

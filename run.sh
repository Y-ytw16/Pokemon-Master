#!/usr/bin/bash

mkdir -p build
g++ -std=c++20 src/main.cpp -o build/main
./build/main

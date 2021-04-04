#!/bin/bash

find src -type f -name "*.cpp" -execdir clang-format -i '{}' ';'
find include -type f -name "*.h" -execdir clang-format -i '{}' ';'
find cmake -type f -name "*.cmake" -execdir cmake-format -i '{}' ';'
find . -type f -name "CMakeLists.txt" -execdir cmake-format -i '{}' ';'
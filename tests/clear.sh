#!/bin/bash

rm -R CMakeCache.txt CMakeFiles/ cmake_install.cmake Makefile
cmake .
make -j7

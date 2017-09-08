#!/bin/bash
# Removes all automatatically-created cmake files
# Useful when need to re-cmake (e.g. change in CMakeLists or toolchain-rpi)

cd $(pwd)  # change to directory the script is being called in
rm CMakeCache.txt cmake_install.cmake Makefile -r CMakeFiles/

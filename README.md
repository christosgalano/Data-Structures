# Data-Structures

[![CMake](https://github.com/christosgalano/DataStructures/actions/workflows/cmake.yml/badge.svg?branch=master)](https://github.com/christosgalano/DataStructures/actions/workflows/cmake.yml)

## General

This project provides a library of several data structures implementations and also tests that ensure their proper functionality.
The tests are based on the GTest framework.

## Setup

``` bash
# Run these commands to install cmake and compiler (g++)
>> sudo apt-get update
>> sudo apt-get install cmake
>> sudo apt-get install g++
```

## Build and Run Manually

The script folder contains all the scripts that are needed to automatically build (and run) the project with cmake

```bash
# Build project with specific build type (debug/release) and testing (ON/OFF)
>> cmake -B ./build -DENABLE_TESTING=ON -DCMAKE_BUILD_TYPE=debug

>> cmake --build ./build  --config debug

# Run tests
>> cd ./build
>> ctest -C debug

# Cleanup
>> rm -rf build;
```

# Data-Structures

[![build-analyze-test-upload](https://github.com/christosgalano/DataStructures/actions/workflows/build-analyze-test-upload.yaml/badge.svg?branch=main)](https://github.com/christosgalano/DataStructures/actions/workflows/build-analyze-test-upload.yaml)

## General

This project provides a library of several data structures implementations and also tests that ensure their proper functionality.
The tests are based on the GTest framework.

## Setup

``` bash
sudo apt-get update
sudo apt-get install cmake
sudo apt-get install g++
```

## Build and Test

```bash
cmake -B ./build -DENABLE_TESTING=ON -DCMAKE_BUILD_TYPE=debug
cmake --build ./build  --config debug
cd ./build
make install
ctest -C debug
```

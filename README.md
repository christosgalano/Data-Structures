# Documentation

This project provides a library of several data structures implementations and also tests that ensure their proper functionality.
The tests are based on the GTest framework.

## Setup
``` bash
# Run these commands to install cmake and compiler (g++)
>> sudo apt-get update
>> sudo apt-get install cmake
>> sudo apt-get install g++
```

## Build and Run
The script folder contains all the scripts that are needed to automatically build (and run) the project with cmake
``` bash
>> cd scripts

# Build project with specified build type (default=debug)
>> ./build.sh --build_type=release --build_tests=ON

# Build project with testing ON, where the tests excectutables are installed in: dist/BUILD_TYPE/bin
>> ./build.sh --build_tests=ON

# If you have already built the project and just want to make changes to the test files,
# then simply run the following script in order the changes to take place
>> ./make.sh

# Run all tests
>> ./run.sh

# Remove all generated folders (build and dist)
>> ./clean.sh 
```

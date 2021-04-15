#!/usr/bin/env bash

current=`pwd`
current_dir=`basename "$current"`
if [ "$current_dir" == "scripts" ]; then
  cd ..
fi

if [ -d "build/" ]; then
    cd build
else
    echo "Error: build not found, run build.sh to build the project first"
    exit 1;
fi

if [ -d "debug/" ]; then
   cd debug
else
    cd release
fi

make
make install
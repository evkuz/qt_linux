#!/bin/bash

__build_dir="build"

if [ ! -d $__build_dir ]; then
    mkdir -p $__build_dir
fi

cd $__build_dir
cmake ../src && make


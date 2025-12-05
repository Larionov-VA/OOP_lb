#!/bin/bash

mkdir build
mkdir saves
cd build
cmake ..
make -j$(nproc)
./game

#!/bin/bash

# Clean old build files
rm -Rf build/
rm -Rf bin/

# Generate makefiles
utils/premake5 gmake

# Build!
cd build/
make config=release_x32 all
make config=release_x64 all

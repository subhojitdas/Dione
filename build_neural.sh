#!/bin/bash

set -e

# Create a separate build directory specifically for neural
mkdir -p build_neural
cd build_neural

# Remove any existing CMake cache
rm -rf CMakeCache.txt CMakeFiles/

# Configure with CMake
cmake ../src/neural

# Build
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 2)

echo ""
echo "Build completed successfully!"
echo "The neural network library has been compiled."
echo "To run the neural network test, execute: ./neural_test"

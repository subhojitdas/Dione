#!/bin/bash

set -e

# Create a separate build directory specifically for matrix
mkdir -p build_matrix
cd build_matrix

# Remove any existing CMake cache
rm -rf CMakeCache.txt CMakeFiles/

# Configure with CMake
cmake ../src/matrix

# Build
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 2)

echo ""
echo "Build completed successfully!"
echo "The matrix library has been compiled."
echo "To run the matrix test, execute: ./matrix_test"

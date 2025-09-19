#!/bin/bash
set -e  # Exit on first error

echo "=== Plugin System Demo Build Script ==="
echo "Cleaning build directory..."
rm -rf build
mkdir -p build

echo "Configuring project with CMake..."
cmake -S . -B build

echo "Building project..."
cmake --build build -j

echo "Build complete!"
echo ""
echo "To run the plugin demo, execute:"
echo "./build/plugin_loader"

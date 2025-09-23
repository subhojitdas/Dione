#!/bin/bash
set -e  # Exit on first error

echo "=== AWS Bedrock Plugin Build Script ==="
echo "This script builds the AWS Bedrock plugin using the real AWS SDK."
echo "Make sure you have installed the AWS SDK for C++ and configured your AWS credentials."
echo ""

# Check if AWS SDK is installed
if ! pkg-config --exists aws-cpp-sdk-core; then
    echo "WARNING: AWS SDK for C++ might not be installed or not in the pkg-config path."
    echo "The build may fail if the SDK is not properly installed."
    echo ""
    echo "Do you want to continue anyway? (y/n)"
    read -r response
    if [[ ! "$response" =~ ^[Yy]$ ]]; then
        echo "Exiting. Please install the AWS SDK first."
        exit 1
    fi
fi

echo "Cleaning build directory..."
rm -rf build
mkdir -p build

echo "Configuring project with CMake (with AWS SDK)..."
cmake -S . -B build -DBUILD_AWS_MODULE=ON -DUSE_MOCK_BEDROCK=OFF

echo "Building project..."
cmake --build build -j

echo "Build complete!"
echo ""
echo "Before running the AWS Bedrock client, make sure you have:"
echo "1. AWS credentials configured (~/.aws/credentials or environment variables)"
echo "2. Appropriate permissions to access AWS Bedrock services"
echo ""
echo "To run the AWS Bedrock demo, execute:"
echo "./build/bedrock_client"
echo ""
echo "Note: This will make real API calls to AWS Bedrock, which may incur costs."

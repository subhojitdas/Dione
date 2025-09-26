cmake_minimum_required(VERSION 3.16)
project(neural_lib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# First, we need to include the matrix library
# Assuming the matrix library is in a sibling directory
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../matrix ${CMAKE_BINARY_DIR}/matrix)

# Neural network library
add_library(neural INTERFACE)
target_include_directories(neural INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(neural INTERFACE matrix)

# Neural network test application
add_executable(neural_test
    neural_test.cpp
)
target_link_libraries(neural_test PRIVATE neural matrix)

# Set output directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

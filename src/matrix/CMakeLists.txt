cmake_minimum_required(VERSION 3.16)
project(matrix_lib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Matrix library
add_library(matrix STATIC
    matrix.cpp
)
target_include_directories(matrix PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Matrix test application
add_executable(matrix_test
    matrix_test.cpp
)
target_link_libraries(matrix_test PRIVATE matrix)

# Set output directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

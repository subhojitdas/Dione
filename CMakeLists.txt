cmake_minimum_required(VERSION 3.16)
project(cpp_systems_playground LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_library(hello STATIC
        src/hello/hello.cpp
)
target_include_directories(hello PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_executable(app
        src/hello/main.cpp
)
target_link_libraries(app PRIVATE hello)

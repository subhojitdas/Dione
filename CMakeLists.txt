cmake_minimum_required(VERSION 3.16)
project(cpp_systems_playground LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Set the output directory for plugins
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Original hello library and app
add_library(hello STATIC
        src/hello/hello.cpp
)
target_include_directories(hello PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_executable(app
        src/hello/main.cpp
)
target_link_libraries(app PRIVATE hello)

# Plugin system
# Shared library for the math plugin
add_library(math_plugin SHARED
        src/plugin/math_plugin.cpp
)
target_include_directories(math_plugin PRIVATE ${CMAKE_SOURCE_DIR}/src/plugin)

# Plugin loader application
add_executable(plugin_loader
        src/plugin/main.cpp
        src/plugin/plugin_loader.cpp
)
target_include_directories(plugin_loader PRIVATE ${CMAKE_SOURCE_DIR}/src/plugin)

# Link with dl library for dynamic loading on Unix systems
if(UNIX AND NOT APPLE)
    target_link_libraries(plugin_loader PRIVATE dl)
elseif(APPLE)
    target_link_libraries(plugin_loader PRIVATE "-framework CoreFoundation")
endif()

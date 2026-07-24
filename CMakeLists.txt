cmake_minimum_required(VERSION 3.12)

project(
    int2025
    VERSION 1.0
    DESCRIPTION "int_2025t proj"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
add_subdirectory(lib)
add_subdirectory(bin)

enable_testing()
add_subdirectory(tests)

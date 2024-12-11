cmake_minimum_required(VERSION 3.21)

project(common)

include("${CMAKE_SOURCE_DIR}/cmake/VC-LTL helper for cmake.cmake")

add_library(common STATIC
    byte_pattern.cpp
    patch.cpp
)

target_link_libraries(common PRIVATE winmm)

target_include_directories(common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

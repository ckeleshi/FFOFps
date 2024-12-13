cmake_minimum_required(VERSION 3.21)

project(FFOFPS)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_subdirectory(common)
add_subdirectory(d3d8)
add_subdirectory(d3d9)
add_subdirectory(ddraw)
add_subdirectory(opengl32)

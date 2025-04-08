cmake_minimum_required(VERSION 3.23.1 FATAL_ERROR)

project(dmvm LANGUAGES C)

find_package(MPI REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/EXE)
add_subdirectory(header)
add_subdirectory(main)

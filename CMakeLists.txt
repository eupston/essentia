cmake_minimum_required(VERSION 3.16)
project(Essentia)

include(ExternalProject)
include(CMake/CPM.cmake)

set(CMAKE_CXX_STANDARD 14)

add_library(${PROJECT_NAME} INTERFACE)

add_subdirectory("src")

add_executable("${PROJECT_NAME}_EXEC" essentia_exec.cpp)
target_link_libraries("${PROJECT_NAME}_EXEC" essentia)
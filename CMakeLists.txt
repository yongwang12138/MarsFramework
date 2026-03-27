cmake_minimum_required(VERSION 3.21)
project(MarsFrameworkWorkspace LANGUAGES CXX)

option(MARS_BUILD_QUICKTOOLS "Build MarsQuickTools subproject" ON)
option(MARS_BUILD_QUICKTOOLS_EXAMPLE "Build MarsQuickToolsExample subproject" ON)

if (MARS_BUILD_QUICKTOOLS)
    add_subdirectory(MarsQuickTools)
endif()

if (MARS_BUILD_QUICKTOOLS_EXAMPLE)
    add_subdirectory(MarsQuickToolsExample)
endif()

message(STATUS "MarsFramework workspace loaded.")
message(STATUS "  MARS_BUILD_QUICKTOOLS         = ${MARS_BUILD_QUICKTOOLS}")
message(STATUS "  MARS_BUILD_QUICKTOOLS_EXAMPLE = ${MARS_BUILD_QUICKTOOLS_EXAMPLE}")

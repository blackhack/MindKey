project(MindKey)

cmake_minimum_required(VERSION 2.8.9)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

set(CMAKE_BUILD_TYPE "Release")

# set macro-directory
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/macros")

include(CheckPlatform)
include(IncludeBoost)

# add sources
add_subdirectory(src)

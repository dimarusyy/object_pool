cmake_minimum_required (VERSION 3.14)

set(LIBRARY_NAME "object-pool")
project (object-pool)

#enable verbose makefiles
set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "ON")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/dependencies.cmake)

add_library(${LIBRARY_NAME} INTERFACE)
target_sources(${LIBRARY_NAME} INTERFACE "include/pool.h")
target_include_directories(${LIBRARY_NAME} INTERFACE  "${PROJECT_SOURCE_DIR}/include")

add_subdirectory(tests)
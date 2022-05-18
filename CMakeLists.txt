cmake_minimum_required(VERSION 3.18)
project(TESLA_LRU LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -m64 -g3 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -m64 -O3 -DNDEBUG")

include_directories(include)
add_subdirectory(src)

add_subdirectory(googletest)
option(BP_BUILD_TESTS "Build tests" ON)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BP_BUILD_TESTS)
    include(GoogleTest)
    enable_testing()
    add_subdirectory(test)
endif()
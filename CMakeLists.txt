cmake_minimum_required(VERSION 3.18)
project(TESLA_LRU LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)

set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall -m64 -g3 -O0 -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall -m64 -O3")

include_directories(include)
add_subdirectory(src)

# option(BP_BUILD_TESTS "Build tests" ON)
# if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BP_BUILD_TESTS)
#     enable_testing()
#     add_subdirectory(test)
# endif()
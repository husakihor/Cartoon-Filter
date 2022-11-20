cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
include(FetchContent)
project(tifo CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -O3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(ZLIB_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/libs/zlib/libz${CMAKE_SHARED_LIBRARY_SUFFIX})
MESSAGE(${CMAKE_CURRENT_SOURCE_DIR}/libs/zlib/libz${CMAKE_SHARED_LIBRARY_SUFFIX})
set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/zlib)
add_subdirectory(${ZLIB_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR})
set(LIBPNG_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/libpng)
add_subdirectory(${LIBPNG_INCLUDE_DIR} ${LIBPNG_INCLUDE_DIR})
include_directories(src ${LIBPNG_INCLUDE_DIR})

add_executable(tifo
        src/image/png.cpp
	src/image/hsl.cpp
        src/image/color.cpp
        src/image/tifo.cpp
        src/main.cpp
        src/filters/kuwahara.cpp
        src/filters/sobel.cpp
        src/filters/medianBlur.cpp
        src/filters/threshold.cpp)

target_link_libraries(tifo zlib png)
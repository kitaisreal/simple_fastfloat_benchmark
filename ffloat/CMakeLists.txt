cmake_minimum_required(VERSION 3.8)
project(ffloat)

set(CMAKE_CXX_STANDARD 17)

set(LIBRARY_HEADERS
    include/ffloat/ffloat.h
    include/ffloat/defines.h
    include/ffloat/shift10.h
    include/ffloat/string_utils.h)

set(LIBRARY_SOURCES
    src/shift10.cpp
    )

add_library(ffloat ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
target_include_directories(ffloat PUBLIC include)
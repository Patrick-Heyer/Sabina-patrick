cmake_minimum_required(VERSION 2.8)
PROJECT(Gestures)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib)

include_directories(/usr/lib64/gcc/x86_64-pc-linux-gnu/4.6.3/include/g++-v4/)
include_directories(/usr/lib64/gcc/x86_64-pc-linux-gnu/4.6.3/include/g++-v4/x86_64-pc-linux-gnu/)

add_library(Gestures SHARED main.cpp observations.cpp dynamic_win.cpp hmm.cpp)
target_link_libraries(Gestures pthread )
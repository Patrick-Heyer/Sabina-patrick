cmake_minimum_required(VERSION 2.8)
PROJECT(Voice_Recognition)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib)

add_library(Voice_Recognition  SHARED main.cpp tty-continuous.cpp )
target_link_libraries(Voice_Recognition  coreapi interface shared_memory sphinx2 sphinx2ad sphinx2fe ${CMAKE_THREAD_LIBS_INIT} )

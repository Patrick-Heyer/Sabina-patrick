cmake_minimum_required(VERSION 2.8)
PROJECT(Voice_Synthesis)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib)

add_library(Voice_Synthesis  SHARED main.cpp )
target_link_libraries(Voice_Synthesis  coreapi interface shared_memory ${CMAKE_THREAD_LIBS_INIT} )

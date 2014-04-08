cmake_minimum_required(VERSION 2.8)
PROJECT(Sequencer)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib)

add_library(Sequencer SHARED main.cpp )
target_link_libraries(Sequencer coreapi interface shared_memory ${CMAKE_THREAD_LIBS_INIT} )


cmake_minimum_required(VERSION 2.8)



add_library(voice_synthesis  SHARED main.cpp )
target_link_libraries(voice_synthesis  coreapi interface shared_memory ${CMAKE_THREAD_LIBS_INIT} )

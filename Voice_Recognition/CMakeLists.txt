cmake_minimum_required(VERSION 2.8)



add_library(voice_recognition  SHARED main.cpp tty-continuous.cpp )
target_link_libraries(voice_recognition  coreapi interface shared_memory sphinx2 sphinx2ad sphinx2fe ${CMAKE_THREAD_LIBS_INIT} )

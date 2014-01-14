cmake_minimum_required(VERSION 2.8)
PROJECT(Prueba_Dummy)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib)

include_directories(/usr/local/include/player-3.0/ )

add_library(Prueba_Dummy SHARED main.cpp )
target_link_libraries(Prueba_Dummy coreapi interface shared_memory ${CMAKE_THREAD_LIBS_INIT} )


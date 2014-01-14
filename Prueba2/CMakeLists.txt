
cmake_minimum_required(VERSION 2.8)



add_library(prueba2 SHARED plugin2.cpp )
target_link_libraries(prueba2 coreapi interface ${CMAKE_THREAD_LIBS_INIT} )
cmake_minimum_required(VERSION 2.8)
PROJECT(Navegation_Restaurant)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib)

include_directories(/usr/local/Aria/include)
link_directories(/usr/local/Aria/lib/)

add_library(Navegation_Restaurant SHARED main.cpp Controller.cpp  )
target_link_libraries(Navegation_Restaurant coreapi interface  Aria ArNetworking rt dl ${CMAKE_THREAD_LIBS_INIT} )


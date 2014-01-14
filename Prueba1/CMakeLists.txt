
cmake_minimum_required(VERSION 2.8)

FIND_PACKAGE( OpenCV REQUIRED )

add_library(prueba1 SHARED plugin1.cpp  )
target_link_libraries(prueba1 coreapi interface  ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} )
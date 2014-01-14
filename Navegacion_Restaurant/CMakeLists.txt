cmake_minimum_required(VERSION 2.8)

include_directories(/usr/local/Aria/include)
link_directories(/usr/local/Aria/lib/)

add_library(navegacion_restaurant SHARED main.cpp Controller.cpp  )
target_link_libraries(navegacion_restaurant coreapi interface  Aria ArNetworking rt dl ${CMAKE_THREAD_LIBS_INIT} )


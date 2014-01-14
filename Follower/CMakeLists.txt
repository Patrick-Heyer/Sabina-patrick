cmake_minimum_required(VERSION 2.8)

include_directories(/usr/local/Aria/include)
link_directories(/usr/local/Aria/lib/)

add_library(Follower SHARED main.cpp )
target_link_libraries(Follower coreapi interface  Aria ArNetworking rt dl ${CMAKE_THREAD_LIBS_INIT} )


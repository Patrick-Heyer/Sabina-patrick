cmake_minimum_required(VERSION 2.2 FATAL_ERROR)
project(humo)

find_package( OpenCV REQUIRED )

add_executable(humo main.cpp NetThread.cpp gsNetPoll_g++.cpp)

target_link_libraries(humo ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT}   opencv_nonfree ) 

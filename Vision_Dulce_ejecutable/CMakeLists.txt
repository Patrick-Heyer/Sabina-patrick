cmake_minimum_required(VERSION 2.2 FATAL_ERROR)
project(vision_dulce_ejecutable)

find_package( OpenCV REQUIRED )

add_executable(vision_dulce_ejecutable inicial.cpp cvtexture_demo.cpp classifiers.cpp  NetThread.cpp gsNetPoll_g++.cpp)

target_link_libraries(vision_dulce_ejecutable ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT}   opencv_nonfree ) 

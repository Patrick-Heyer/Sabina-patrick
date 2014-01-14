cmake_minimum_required(VERSION 2.2 FATAL_ERROR)
project(vision_dulce)

find_package( OpenCV REQUIRED )

add_library(vision_dulce SHARED main.cpp )

target_link_libraries(vision_dulce coreapi interface shared_memory ${CMAKE_THREAD_LIBS_INIT}  opencv_nonfree  ${OpenCV_LIBS})
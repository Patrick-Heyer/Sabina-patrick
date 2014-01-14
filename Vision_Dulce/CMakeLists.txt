cmake_minimum_required(VERSION 2.8)
PROJECT(Vision_Dulce)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib)

find_package( OpenCV REQUIRED )

add_library(Vision_Dulce SHARED main.cpp )
target_link_libraries(Vision_Dulce coreapi interface shared_memory ${CMAKE_THREAD_LIBS_INIT}  opencv_nonfree  ${OpenCV_LIBS})
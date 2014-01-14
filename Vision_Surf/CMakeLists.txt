cmake_minimum_required(VERSION 2.8)
PROJECT(Vision_Surf)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib)

find_package( OpenCV REQUIRED )

add_library(Vision_Surf SHARED main.cpp surfrecognizer.cpp)
target_link_libraries(Vision_Surf coreapi interface shared_memory ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} )


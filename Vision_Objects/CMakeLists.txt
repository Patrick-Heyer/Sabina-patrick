cmake_minimum_required(VERSION 2.8)
PROJECT(Vision_Objects)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib)

find_package( OpenCV REQUIRED )


add_library(Vision_Objects SHARED main.cpp HC.cpp recognizer.cpp )
target_link_libraries(Vision_Objects coreapi interface shared_memory OpenNI ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} )


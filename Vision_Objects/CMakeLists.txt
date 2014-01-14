cmake_minimum_required(VERSION 2.8)
PROJECT(Vision_Judith)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib)

find_package( OpenCV REQUIRED )

add_library(Vision_Judith SHARED main.cpp comparepoint.cpp histograma.cpp imagelocation.cpp matchingsurf.cpp toolsimages.cpp imagesimilarity.cpp findobject.cpp )
target_link_libraries(Vision_Judith coreapi interface shared_memory ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} )


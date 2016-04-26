cmake_minimum_required(VERSION 2.8)
PROJECT(Vision_Faces)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib)

FIND_PACKAGE( OpenCV REQUIRED )

add_library(Vision_Faces SHARED main.cpp sift.cpp facedetect.cpp imgfeatures.cpp kdtree.cpp  minpq.cpp utils.cpp  learn.cpp load.cpp tratamiento.cpp recognize.cpp )
target_link_libraries(Vision_Faces coreapi interface shared_memory ${OpenCV_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )


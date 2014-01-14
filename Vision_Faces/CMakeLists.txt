cmake_minimum_required(VERSION 2.8)

FIND_PACKAGE( OpenCV REQUIRED )

add_library(vision_faces SHARED main.cpp sift.cpp facedetect.cpp
 imgfeatures.cpp kdtree.cpp  minpq.cpp utils.cpp  learn.cpp load.cpp tratamiento.cpp recognize.cpp
../Gui/console.cpp  ../Gui/widget.cpp ../Gui/tab.cpp ../Gui/video.cpp )
target_link_libraries(vision_faces coreapi interface shared_memory ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} )


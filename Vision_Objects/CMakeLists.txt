cmake_minimum_required(VERSION 2.2 FATAL_ERROR)
project(MINE)
find_package( OpenCV REQUIRED )

add_library(MINE SHARED main.cpp comparepoint.cpp histograma.cpp imagelocation.cpp matchingsurf.cpp toolsimages.cpp imagesimilarity.cpp findobject.cpp )

#TARGET_LINK_LIBRARIES(MINE  opencv_core opencv_imgproc opencv_highgui #opencv_ml opencv_video opencv_features2d opencv_calib3d opencv_objdetect #opencv_contrib opencv_legacy opencv_flann ${EXTERNAL_LIBS_})
target_link_libraries(MINE coreapi interface shared_memory ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} )


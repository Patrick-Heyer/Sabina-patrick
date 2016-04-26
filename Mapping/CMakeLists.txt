
PROJECT(Mapping)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib)

FIND_PACKAGE(MRPT REQUIRED hwdrivers;maps;obs;slam)

find_package(MRPT REQUIRED hwdrivers slam maps obs vision)

find_package( OpenCV REQUIRED )

link_directories(${MRPT_LIBS_INCL_DIR})
link_directories(/usr/local/lib)

include_directories(${MRPT_INCLUDE_DIRS})
include_directories(/usr/local/include/mrpt/)
include_directories(/usr/local/include/mrpt/mrpt-config/)
include_directories(/usr/local/include/mrpt/slam/include/)
include_directories(/usr/local/include/mrpt/maps/include/)
include_directories(/usr/local/include/mrpt/base/include/)
include_directories(/usr/local/include/mrpt/obs/include/)
include_directories(/usr/local/include/mrpt/opengl/include/)
include_directories(/usr/local/include/mrpt/vision/include/)
include_directories(/usr/local/include/mrpt/bayes/include/)
include_directories(/usr/local/include/mrpt/hwdrivers/include/)



add_library (Mapping SHARED  main.cpp )


target_link_libraries (Mapping ${MRPT_LIBS} coreapi interface mrpt-hwdrivers mrpt-maps   ${OpenCV_LIBS}  ${CMAKE_THREAD_LIBS_INIT})




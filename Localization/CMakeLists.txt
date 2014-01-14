cmake_minimum_required(VERSION 2.8)
PROJECT(Localization)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib)

find_package(MRPT REQUIRED hwdrivers slam maps obs vision)

link_directories(${MRPT_LIBS_INCL_DIR})

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

add_library (Localization SHARED  main.cpp )
target_link_libraries (Localization ${MRPT_LIBS} mrpt-hwdrivers mrpt-maps  )




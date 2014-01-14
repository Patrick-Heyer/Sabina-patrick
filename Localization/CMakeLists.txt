
PROJECT(PFLocalization)

cmake_minimum_required(VERSION 2.8)


find_package(MRPT REQUIRED hwdrivers slam maps obs vision)


link_directories(${MRPT_LIBS_INCL_DIR})

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

add_library (PFLocalization SHARED  main.cpp )
target_link_libraries (PFLocalization ${MRPT_LIBS} mrpt-hwdrivers mrpt-maps  )






cmake_minimum_required(VERSION 2.8)

include_directories(/usr/include/ni)


add_library(Kinect SHARED Skeleton_Writer.cpp)
target_link_libraries(Kinect coreapi OpenNI )

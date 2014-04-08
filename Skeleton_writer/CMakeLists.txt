cmake_minimum_required(VERSION 2.8)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
PROJECT(Skeleton_Writer)

include_directories(/usr/include/ni)


add_library(Skeleton_Writer SHARED Skeleton_Writer.cpp)
target_link_libraries(Skeleton_Writer coreapi OpenNI )

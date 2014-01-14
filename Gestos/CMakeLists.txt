cmake_minimum_required(VERSION 2.2 FATAL_ERROR)
project(gestos)

include_directories(/usr/include/ni/)


add_library(gestos SHARED main.cpp NiHandTracker.cpp NiHandViewer.cpp NiSimpleViewer.cpp NiTrailHistory.cpp)

target_link_libraries(gestos coreapi interface shared_memory OpenNI ${CMAKE_THREAD_LIBS_INIT} )



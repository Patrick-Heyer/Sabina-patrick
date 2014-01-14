cmake_minimum_required(VERSION 2.8)
PROJECT(Net_Client)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib)

add_library(Net_Client SHARED main.cpp NetThread.cpp gsNetPoll_g++.cpp)
target_link_libraries(Net_Client   pthread )
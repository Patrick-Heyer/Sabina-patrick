cmake_minimum_required(VERSION 2.8)
PROJECT(Network)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib)

add_library(Server SHARED Server.cpp NetThread.cpp gsNetPoll_g++.cpp)
add_library(Client SHARED Client.cpp NetThread.cpp gsNetPoll_g++.cpp)
target_link_libraries(Server   pthread )
target_link_libraries(Client   pthread )
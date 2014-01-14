PROJECT(vision_server)

add_library(vision_server SHARED main.cpp NetThread.cpp gsNetPoll_g++.cpp)

target_link_libraries(vision_server   pthread )
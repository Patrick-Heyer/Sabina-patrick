PROJECT(server)

add_library(server SHARED main.cpp NetThread.cpp gsNetPoll_g++.cpp)

target_link_libraries(server   pthread )
PROJECT(client)

add_library(client SHARED main.cpp NetThread.cpp gsNetPoll_g++.cpp)

target_link_libraries(client   pthread )
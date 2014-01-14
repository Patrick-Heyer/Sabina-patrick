PROJECT(vision_server)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
add_library(vision_server SHARED main.cpp NetThread.cpp gsNetPoll_g++.cpp)

target_link_libraries(vision_server   pthread )
PROJECT(Navigation_Client)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib)

include_directories(/usr/local/Arnl/include/ArNetworking)
include_directories(/usr/local/Arnl/include/Aria)

include_directories(/usr/local/Aria/ArNetworking/include/)
include_directories(/usr/local/Aria/include/)
link_directories(/usr/local/Aria/lib/)

link_directories(/usr/local/Arnl/lib/)

add_library(Navigation_Client SHARED main.cpp)

target_link_libraries(Navigation_Client Aria ArNetworking rt ${CMAKE_THREAD_LIBS_INIT} )

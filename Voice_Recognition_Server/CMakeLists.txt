cmake_minimum_required(VERSION 2.8)
PROJECT(VoiceRecognitionServer)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib)

include_directories(/usr/include/alsa/)
link_directories(/usr/lib/i386-linux-gnu/)
add_library(VoiceRecognitionServer SHARED main.cpp NetThread.cpp gsNetPoll_g++.cpp)
target_link_libraries(VoiceRecognitionServer pthread asound)
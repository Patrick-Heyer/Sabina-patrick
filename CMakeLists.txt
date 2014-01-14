project(sabina)

# Declare the minimum version of cmake that we need
cmake_minimum_required(VERSION 2.8)

add_subdirectory (Gui)
add_subdirectory (Plugin_API)
add_subdirectory (Shared_memory)

add_subdirectory (Prueba1)
add_subdirectory (Prueba2)

add_executable(sabina main.cpp)

target_link_libraries(sabina interface plugin shared_memory )
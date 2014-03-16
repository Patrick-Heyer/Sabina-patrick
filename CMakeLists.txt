project(sabina)

# Declare the minimum version of cmake that we need
cmake_minimum_required(VERSION 2.8)


FIND_PACKAGE(FLTK REQUIRED)

add_subdirectory (Gui)
add_subdirectory (Human)
add_subdirectory (Humo)
add_subdirectory (Manipulacion)
add_subdirectory (Navegation)
add_subdirectory (Navegation_Planing)
add_subdirectory (Network)
add_subdirectory (Plugin_API)
add_subdirectory (Prueba_Dummy)
add_subdirectory (Shared_memory)
add_subdirectory (Kinect)
add_subdirectory (Vision_Faces)
add_subdirectory (Vision_Objects)
add_subdirectory (Voice_Recognition)
add_subdirectory (Voice_Synthesis)




link_directories(/home/patrick/projects/Sabina/MDP/lib)
link_directories(/home/patrick/projects/Sabina/build/Human)

add_executable(sabina main.cpp ConfigFile.cpp MDP/main.cpp Face/main.cpp  Face/Md3Model.cpp Face/vector3d.cpp Face/PNG.cpp Face/model_3ds.cpp Face/Texture_Manager.cpp Face/TGA.cpp Face/DDS.cpp)

target_link_libraries(sabina interface plugin shared_memory Human mean cudd dddmp epd mtr st terminal cudd mean pquery spudd_pat util spudd fltk fltk_gl png)

## Compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-O2")        ## Optimize
    set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary
endif()





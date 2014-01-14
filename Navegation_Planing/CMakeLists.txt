
cmake_minimum_required(VERSION 2.8)


add_library(navigation_planing SHARED main.cpp read_prm.cpp CAmbiente.cpp CGraph.cpp CLocal.cpp CNearNode.cpp CPersonajeConfig.cpp CProbot.cpp CSampler.cpp CSamples.cpp PlanningParameters.cpp Random.cpp TiposBasicos.cpp Workspace.cpp )
target_link_libraries(navigation_planing coreapi interface png   ${CMAKE_THREAD_LIBS_INIT} )

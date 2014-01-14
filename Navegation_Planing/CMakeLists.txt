cmake_minimum_required(VERSION 2.8)
PROJECT(Navegacion_Planing)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib)

add_library(Navegacion_Planing SHARED main.cpp read_prm.cpp CAmbiente.cpp CGraph.cpp CLocal.cpp CNearNode.cpp CPersonajeConfig.cpp CProbot.cpp CSampler.cpp CSamples.cpp PlanningParameters.cpp Random.cpp TiposBasicos.cpp Workspace.cpp )
target_link_libraries(Navegacion_Planing coreapi interface png   ${CMAKE_THREAD_LIBS_INIT} )

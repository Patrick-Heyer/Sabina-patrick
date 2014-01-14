cmake_minimum_required(VERSION 2.8)
PROJECT(Manipulacion)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib)

find_package(MRPT)

link_directories(${PROJECT_SOURCE_DIR}/../Manipulacion/lib)

set(LIBRARY_DIR ${PROJECT_SOURCE_DIR}/../Manipulacion/lib)
set(BASE_LIBRARY ${LIBRARY_DIR}/libKNIBase.a)
set(KINE_LIBRARY ${LIBRARY_DIR}/libKinematics.a)
set(INVKIN_LIBRARY ${LIBRARY_DIR}/libKNI_InvKin.a)
set(LM_LIBRARY ${LIBRARY_DIR}/libKNI_LM.a)
set(KATANA_LIBRARIES ${LM_LIBRARY} ${INVKIN_LIBRARY} ${KINE_LIBRARY} ${BASE_LIBRARY} )

include_directories(${MRPT_INCLUDE_DIRS})


add_library(Manipulacion SHARED  main.cpp  armkatana.cpp armkatanaforsabina.cpp kintoarmtransf.cpp )
target_link_libraries(Manipulacion coreapi interface   ${CMAKE_THREAD_LIBS_INIT}  ${KATANA_LIBRARIES}  ${LIBS_MRPT} )

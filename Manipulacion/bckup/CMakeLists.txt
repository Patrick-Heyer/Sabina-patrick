cmake_minimum_required(VERSION 2.8)

link_directories(/home/robot/Sabina/Manipulacion/lib)

set(LIBRARY_DIR /home/robot/Sabina/Manipulacion/lib)
set(BASE_LIBRARY ${LIBRARY_DIR}/libKNIBase.a)
set(KINE_LIBRARY ${LIBRARY_DIR}/libKinematics.a)
set(INVKIN_LIBRARY ${LIBRARY_DIR}/libKNI_InvKin.a)
set(LM_LIBRARY ${LIBRARY_DIR}/libKNI_LM.a)

set(KATANA_LIBRARIES ${LM_LIBRARY} ${INVKIN_LIBRARY} ${KINE_LIBRARY} ${BASE_LIBRARY} )

# -----------------  MRPT ------------------------
 find_package(MRPT)
 include_directories(${MRPT_INCLUDE_DIRS})

# set(LIBS_MRPT ${MRPT_LIBRARIES} mrpt-base)
set(SRCS_TEST_ARM main.cpp  armkatana.cpp armkatanaforsabina.cpp)

add_library(manipulacion SHARED  ${SRCS_TEST_ARM} )
target_link_libraries(manipulacion coreapi interface   ${CMAKE_THREAD_LIBS_INIT}  ${KATANA_LIBRARIES}  ${LIBS_MRPT} )

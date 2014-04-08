#---------------------------------------------------------e--------------------------------------
# CMake file for the MRPT custom: myRawlog
#
#  Run with "ccmake ." at the root directory, or use it as a template for 
#   starting your own programs
#-----------------------------------------------------------------------------------------------

# ---------------------------------------
# Declare a new CMake Project:
# ---------------------------------------
PROJECT(Mapping)

# These commands are needed by modern versions of CMake:
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib)

FIND_PACKAGE(MRPT REQUIRED hwdrivers;maps;obs;slam)

find_package(MRPT REQUIRED hwdrivers slam maps obs vision)

find_package( OpenCV REQUIRED )

link_directories(${MRPT_LIBS_INCL_DIR})
link_directories(/usr/local/lib)

include_directories(${MRPT_INCLUDE_DIRS})
include_directories(/usr/local/include/mrpt/)
include_directories(/usr/local/include/mrpt/mrpt-config/)
include_directories(/usr/local/include/mrpt/slam/include/)
include_directories(/usr/local/include/mrpt/maps/include/)
include_directories(/usr/local/include/mrpt/base/include/)
include_directories(/usr/local/include/mrpt/obs/include/)
include_directories(/usr/local/include/mrpt/opengl/include/)
include_directories(/usr/local/include/mrpt/vision/include/)
include_directories(/usr/local/include/mrpt/bayes/include/)
include_directories(/usr/local/include/mrpt/hwdrivers/include/)



add_library (Mapping SHARED  main.cpp )

SET_TARGET_PROPERTIES(Mapping PROPERTIES PROJECT_LABEL "Mapping")

# Add special defines needed by this example, if any:
SET(MY_DEFS )
IF(MY_DEFS) # If not empty
	ADD_DEFINITIONS("-D${MY_DEFS}")
ENDIF(MY_DEFS)

target_link_libraries (Mapping ${MRPT_LIBS} coreapi mrpt-hwdrivers mrpt-maps  ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})



# Set optimized building:
IF(CMAKE_COMPILER_IS_GNUCXX AND NOT CMAKE_BUILD_TYPE MATCHES "Debug")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -mtune=native")
ENDIF(CMAKE_COMPILER_IS_GNUCXX AND NOT CMAKE_BUILD_TYPE MATCHES "Debug")


# -------------------------------------------------------------------------
# This part can be removed if you are compiling this program outside of 
#  the MRPT tree:
# -------------------------------------------------------------------------
IF("${CMAKE_PROJECT_NAME}" STREQUAL "MRPT") # Fails if build outside of MRPT project.
	DeclareAppDependencies(${sampleName} mrpt-hwdrivers;mrpt-maps) # Dependencies
ENDIF("${CMAKE_PROJECT_NAME}" STREQUAL "MRPT")
# -------------------------------------------------------------------------


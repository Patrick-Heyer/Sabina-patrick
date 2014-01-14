
cmake_minimum_required(VERSION 2.8)

FIND_PACKAGE( Threads REQUIRED)


# Using the C:\Program Files\ version of cmake won't build shared libraries
# properly under Cygwin. You must install Cygwin's version of cmake for that.

if ("${CMAKE_GENERATOR}" STREQUAL "Unix Makefiles" AND WIN32 AND NOT CYGWIN)

message("WARNING: plugin_api will not be built unless you use Cygwin's /usr/bin/cmake")

else ()

add_library(plugin SHARED coreapi.cpp pluginapi.cpp pluginmanager.cpp )
target_link_libraries(plugin ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(plugin PROPERTIES DEFINE_SYMBOL BUILDING_CORE )

endif ()
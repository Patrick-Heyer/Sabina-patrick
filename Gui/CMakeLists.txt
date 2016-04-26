
cmake_minimum_required(VERSION 2.8)

FIND_PACKAGE(GLUT REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_library (interface  SHARED button.cpp  video.cpp widget.cpp  input_singleton.cpp textbox.cpp tab.cpp gui.cpp view2d.cpp console.cpp)



target_link_libraries (interface ${GLUT_LIBRARY} ${OPENGL_LIBRARY} ${OpenCV_LIBRARIES} Xmu Xext X11 m )
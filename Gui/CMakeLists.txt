
cmake_minimum_required(VERSION 2.8)

add_library (interface SHARED  video.cpp widget.cpp kinect.cpp input_singleton.cpp tab.cpp gui.cpp view2d.cpp console.cpp)

FIND_PACKAGE(GLUT REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE( OpenCV REQUIRED )

target_link_libraries (interface ${GLUT_LIBRARY} ${OPENGL_LIBRARY} ${OpenCV_LIBS} ${OpenCV_LIBS} freenect_sync freenect_cv  Xmu Xext X11 m png)
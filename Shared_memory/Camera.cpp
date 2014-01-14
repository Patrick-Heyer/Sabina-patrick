
#include "Camera.h"

bool Focus_x_escribible=true;
bool Focus_y_escribible=true;
bool Visual_ID_escribible=true;
bool Image_height_escribible=true;
bool Image_width_escribible=true;

///*! \brief Gets the X coordinate of an object
// *
// *  Gets the X coordinate of the object of interest in the image captured by the camera
// *  \return Focus_x
// */
int Camera::get_Focus_x() {
  return Focus_x;
}

///*! \brief Sets the X coordinate of an object
// *
// *  Sets the X coordinate of the object of interest in the image captured by the camera
// */
void Camera::set_Focus_x(int value) {
  Focus_x = value;
}

///*! \brief Gets the Y coordinate of an object
// *
// *  Gets the Y coordinate of the object of interest in the image captured by the camera
// *  \return Focus_y
// */
int Camera::get_Focus_y() {
  return Focus_y;
}

///*! \brief Sets the Y coordinate of an object
// *
// *  Sets the Y coordinate of the object of interest in the image captured by the camera
// */
void Camera::set_Focus_y(int value) {
  Focus_y = value;
}

///*! \brief Gets the Visual_ID of an object
// *
// *  Gets the Visual_ID of the object of interest in the image captured by the camera
// *  \return Visual_ID
// */
int Camera::get_Visual_ID() {
  return Visual_ID;
}

///*! \brief Sets the Visual_ID of an object
// *
// *  Sets the Visual_ID of the object of interest in the image captured by the camera
// */
void Camera::set_Visual_ID(int value) {
  Visual_ID = value;
}

///*! \brief Gets the height in pixels of an image
// *
// *  Gets the height of the image captured by the camera  \return Image_height
// */
int Camera::get_Image_height() {
  return Image_height;
}

///*! \brief Sets the height in pixels of an image
// *
// *  Sets the height of the image captured by the camera
// */
void Camera::set_Image_height(int value) {
  Image_height = value;
}

///*! \brief Gets the width in pixels of an image
// *
// *  Gets the width of the image captured by the camera  \return Image_width
// */
int Camera::get_Image_width() {
  return Image_width;
}

///*! \brief Sets the width in pixels of an image
// *
// *  Sets the width of the image captured by the camera
// */
void Camera::set_Image_width(int value) {
  Image_width = value;
}


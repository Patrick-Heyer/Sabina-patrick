
#include "Camera.h"

bool Image_height_escribible=true;
bool Image_width_escribible=true;


///*! \brief Gets the height in pixels of an image
// *
// *  Gets the height of the image captured by the camera  \return Image_height
// */
int Camera::getImageHeight() {
    return imageHeight;
}

///*! \brief Sets the height in pixels of an image
// *
// *  Sets the height of the image captured by the camera
// */
void Camera::setImageHeight(int value) {
    imageHeight = value;
}

///*! \brief Gets the width in pixels of an image
// *
// *  Gets the width of the image captured by the camera  \return Image_width
// */
int Camera::getImageWidth() {
    return imageWidth;
}

///*! \brief Sets the width in pixels of an image
// *
// *  Sets the width of the image captured by the camera
// */
void Camera::setImageWidth(int value) {
    imageWidth = value;
}


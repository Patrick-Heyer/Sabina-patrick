#ifndef _CAMERA_H
#define _CAMERA_H

#include <opencv/cv.h>
#include <opencv/highgui.h>

///*! \brief Information about the processed information of a camara
// *
// *
// */
class Camera {
private:
    ///*! \brief The height in pixels of the image
    // *
    // *
    // */
    int imageHeight;

    ///*! \brief The width in pixels of the image
    // *
    // *
    // */
    int imageWidth;


public:

    ///*! \brief Gets the height in pixels of an image
    // *
    // *  Gets the height of the image captured by the camera  \return Image_height
    // */
    int getImageHeight();

    ///*! \brief Sets the height in pixels of an image
    // *
    // *  Sets the height of the image captured by the camera
    // */
    void setImageHeight(int value);

    ///*! \brief Gets the width in pixels of an image
    // *
    // *  Gets the width of the image captured by the camera  \return Image_width
    // */
    int getImageWidth();

    ///*! \brief Sets the width in pixels of an image
    // *
    // *  Sets the width of the image captured by the camera
    // */
    void setImageWidth(int value);

    friend class Robot;
};
#endif

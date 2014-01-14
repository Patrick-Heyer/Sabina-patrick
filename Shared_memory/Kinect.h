#ifndef _KINECT_H
#define _KINECT_H


#include "Camera.h"

///*! \brief Information about the processed information of a kinect
// *
// *
// */
class Kinect : public Camera {
private:
    IplImage * depth;
    IplImage * color;
    int device;

    ///*! \brief The Z coordinate of an object in the depth image
    // *
    // *
    // */
    int Focus_z;

    ///*! \brief The tilt value of the camera
    // *
    // *
    // */
    float Tilt;


public:
    void set_device(int value);
    void set_RGB(IplImage *data);
    void set_depth(IplImage *data);
    IplImage *get_RGB();
    IplImage *get_depth();
    ///*! \brief Gets the Z coordinate of an object
    // *
    // *  Gets the Z coordinate of the object of interest in the image captured by the camera
    // *  \return Focus_z
    // */
    int get_Focus_z();

    ///*! \brief Sets the Z coordinate of an object
    // *
    // *  Sets the Z coordinate of the object of interest in the image captured by the camera
    // */
    void set_Focus_z(int value);

    ///*! \brief Gets the tilt value of the kinect
    // *
    // *  Gets the tilt value of the kinect \return Tilt
    // */
    float get_Tilt();

    ///*! \brief Sets the tilt value of the kinect
    // *
    // *  Sets the tilt value of the kinect
    // */
    void set_Tilt(float value);

    friend class Camera;
};
#endif

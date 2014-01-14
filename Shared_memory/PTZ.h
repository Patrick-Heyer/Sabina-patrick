#ifndef _PTZ_H
#define _PTZ_H


#include "Camera.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

///*! \brief Information about the processed information of a PTZ camara
// *
// *
// */
class PTZ : public Camera {
private:
    CvCapture *stream;
    IplImage *frame;


    ///*! \brief The tilt value of the camera
    // *
    // *
    // */
    float Tilt;

    ///*! \brief The pan value of the camera
    // *
    // *
    // */
    float Pan;

    ///*! \brief The zoom value of the camera
    // *
    // *
    // */
    float Zoom;


public:
    void set_stream(CvCapture *value);
    IplImage *get_image();

    ///*! \brief Gets the tilt value of the camera
    // *
    // *  Gets the tilt value of the camera \return Tilt
    // */
    float get_Tilt();

    ///*! \brief Sets the tilt value of the camera
    // *
    // *  Sets the tilt value of the camera
    // */
    void set_Tilt(float value);

    ///*! \brief Gets the pan value of the camera
    // *
    // *  Gets the pan value of the camera \return Pan
    // */
    float get_Pan();

    ///*! \brief Sets the pan value of the camera
    // *
    // *  Sets the pan value of the camera \return Tilt
    // */
    void set_Pan(float value);

    ///*! \brief Gets the zoom value of the camera
    // *
    // *  Gets the zoom value of the camera \return Zoom
    // */
    float get_Zoom();

    ///*! \brief Sets the zoom value of the camera
    // *
    // *  Sets the tilt value of the camera
    // */
    void set_Zoom(float value);

    friend class Camera;
};
#endif

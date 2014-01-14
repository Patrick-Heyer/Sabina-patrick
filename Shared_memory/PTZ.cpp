
#include "PTZ.h"
#include "Robot.h"

Robot *patrol;

bool Tilt_escribible=true;
bool Pan_escribible=true;
bool Zoom_escribible=true;
bool Video_escribible=true;

void PTZ::set_stream(CvCapture* value)
{
    stream=value;
}

IplImage* PTZ::get_image()
{
    do{

    }while(!Video_escribible);
    IplImage *img1;
    Video_escribible=false;
    img1 = cvCloneImage(cvQueryFrame(stream));
    Video_escribible=true;
    return img1;
}


///*! \brief Gets the tilt value of the camera
// *
// *  Gets the tilt value of the camera \return Tilt
// */
float PTZ::get_Tilt() {
  return Tilt;
}

///*! \brief Sets the tilt value of the camera
// *
// *  Sets the tilt value of the camera
// */
void PTZ::set_Tilt(float value) {
  do
  {
  }while (!Tilt_escribible);
  Tilt_escribible=false;
  Tilt = value;
  Tilt_escribible=true;
}

///*! \brief Gets the pan value of the camera
// *
// *  Gets the pan value of the camera \return Pan
// */
float PTZ::get_Pan() {
  return Pan;
}

///*! \brief Sets the pan value of the camera
// *
// *  Sets the pan value of the camera \return Tilt
// */
void PTZ::set_Pan(float value) {
  do
  {
  }while (!Pan_escribible);
  Pan_escribible=false;
  Pan = value;
  Pan_escribible=true;
}

///*! \brief Gets the zoom value of the camera
// *
// *  Gets the zoom value of the camera \return Zoom
// */
float PTZ::get_Zoom() {
  return Zoom;
}

///*! \brief Sets the zoom value of the camera
// *
// *  Sets the tilt value of the camera 
// */
void PTZ::set_Zoom(float value) {
  do
  {
  }while (!Zoom_escribible);
  Zoom_escribible=false;
  Zoom = value;
  Zoom_escribible=true;
}


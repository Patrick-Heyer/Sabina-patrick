
#include "Kinect.h"
#include <libfreenect/libfreenect_cv.h>
#include <libfreenect/libfreenect_sync.h>

bool Focus_z_escribible=true;
bool Tilt_ki_escribible=true;

void Kinect::set_device(int value)
{
    device=value;
}

IplImage* Kinect::get_depth()
{
    return freenect_sync_get_depth_cv(device);
}

IplImage* Kinect::get_RGB()
{
    IplImage * image;
    image=freenect_sync_get_rgb_cv(0);
    cvCvtColor(image,image,CV_RGB2BGR);
    return image;
}



///*! \brief Gets the Z coordinate of an object
// *
// *  Gets the Z coordinate of the object of interest in the image captured by the camera
// *  \return Focus_z
// */
int Kinect::get_Focus_z() {
  return Focus_z;
}

///*! \brief Sets the Z coordinate of an object
// *
// *  Sets the Z coordinate of the object of interest in the image captured by the camera
// */
void Kinect::set_Focus_z(int value) {
  do
  {
  }while (!Focus_z_escribible);
  Focus_z_escribible=false;
  Focus_z = value;
  Focus_z_escribible=true;
}

///*! \brief Gets the tilt value of the kinect
// *
// *  Gets the tilt value of the kinect \return Tilt
// */
float Kinect::get_Tilt() {
  return Tilt;
}

///*! \brief Sets the tilt value of the kinect
// *
// *  Sets the tilt value of the kinect
// */
void Kinect::set_Tilt(float value) {
  do
  {
  }while (!Tilt_ki_escribible);
  Tilt_ki_escribible=false;
  Tilt = value;
  Tilt_ki_escribible=true;
}


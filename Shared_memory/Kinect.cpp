
#include "Kinect.h"
bool Focus_z_escribible=true;
bool Tilt_ki_escribible=true;

bool cbuffer=0;
bool dbuffer=0;

Kinect::Kinect()
{
    depth0=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
    color0=cvCreateImage(cvSize(1280,1024),IPL_DEPTH_8U,3);
    depth1=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
    color1=cvCreateImage(cvSize(1280,1024),IPL_DEPTH_8U,3);
}


void Kinect::set_device(int value)
{
    device=value;
}

void Kinect::set_depth(IplImage * data)
{
    if(dbuffer && data!=NULL)
    {
        cvCopy(data,depth0);
        dbuffer=0;
    }
    if(!dbuffer && data!=NULL)
    {
        cvCopy(data,depth1);
        dbuffer=1;
    }

}

void Kinect::set_RGB(IplImage * data)
{
    if(cbuffer && data!=NULL)
    {
        cvCopy(data,color0);
        cbuffer=0;
    }
    if(!cbuffer && data!=NULL)
    {
        cvCopy(data,color1);
        cbuffer=1;
    }
}
IplImage* Kinect::get_depth()
{
    if(dbuffer)
        return depth1;
    if(!dbuffer)
        return depth0;
}

IplImage* Kinect::get_RGB()
{
    if(cbuffer)
        return color1;
    if(!cbuffer)
        return color0;
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
    } while (!Focus_z_escribible);
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
    } while (!Tilt_ki_escribible);
    Tilt_ki_escribible=false;
    Tilt = value;
    Tilt_ki_escribible=true;
}


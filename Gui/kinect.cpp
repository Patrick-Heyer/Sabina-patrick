/*
 * < *one line to* give the program's name and a brief idea of what it does.>
 * Copyright (C) 2011  <Patrick Heyer> <patrickhey@prodigy.net.mx>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <opencv/cv.h>
#include <opencv/highgui.h>

//#include <libfreenect/libfreenect_cv.h>

#include "video.h"
#include "kinect.h"
#include "tab.h"

GLuint polo=2;

kinect::kinect(int x, int y, int widht, int height, std::string name, tab* parent)
{
    this->SetName(name);
    this->SetPosition(x, y);
    this->SetSize(widht, height);
    this->mode=0;
    glGenTextures( 1, &this->depth_texture  );
    glBindTexture( GL_TEXTURE_2D, this->depth_texture );
    glEnable(GL_TEXTURE_2D);
    // These are necessary if using glTexImage2D instead of gluBuild2DMipmaps
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    glGenTextures( 1, &this->rgb_texture  );
    glBindTexture( GL_TEXTURE_2D, this->rgb_texture );
    glEnable(GL_TEXTURE_2D);
    // These are necessary if using glTexImage2D instead of gluBuild2DMipmaps
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    parent->InsertWidget(this);
}

void kinect::Update()
{

}

IplImage *GlViewColor(IplImage *depth)
{
    static IplImage *image = 0;
    if (!image) image = cvCreateImage(cvSize(640,480), 8, 3);
    unsigned char *depth_mid = (unsigned char*)(image->imageData);
    int i;
    for (i = 0; i < 640*480; i++) {
        int lb = ((short *)depth->imageData)[i] % 256;
        int ub = ((short *)depth->imageData)[i] / 256;
        switch (ub) {
        case 0:
            depth_mid[3*i+2] = 255;
            depth_mid[3*i+1] = 255-lb;
            depth_mid[3*i+0] = 255-lb;
            break;
        case 1:
            depth_mid[3*i+2] = 255;
            depth_mid[3*i+1] = lb;
            depth_mid[3*i+0] = 0;
            break;
        case 2:
            depth_mid[3*i+2] = 255-lb;
            depth_mid[3*i+1] = 255;
            depth_mid[3*i+0] = 0;
            break;
        case 3:
            depth_mid[3*i+2] = 0;
            depth_mid[3*i+1] = 255;
            depth_mid[3*i+0] = lb;
            break;
        case 4:
            depth_mid[3*i+2] = 0;
            depth_mid[3*i+1] = 255-lb;
            depth_mid[3*i+0] = 255;
            break;
        case 5:
            depth_mid[3*i+2] = 0;
            depth_mid[3*i+1] = 0;
            depth_mid[3*i+0] = 255-lb;
            break;
        default:
            depth_mid[3*i+2] = 0;
            depth_mid[3*i+1] = 0;
            depth_mid[3*i+0] = 0;
            break;
        }
    }
    return image;
}

IplImage* GlViewCode(IplImage *depth)
{
    static IplImage *image = 0;
    if (!image) image = cvCreateImage(cvSize(640,480), 8, 3);
    unsigned char *depth_mid = (unsigned char*)(image->imageData);
    int i;

    for (i = 0; i < 480*640; i++) {
        int lb = ((short *)depth->imageData)[i]/8;
        depth_mid[3*i+2] = lb;
        depth_mid[3*i+1] = lb;
        depth_mid[3*i+0] = lb;

    }
    return image;
}

void kinect::UpdateDepth(IplImage * value)
{
    glBindTexture( GL_TEXTURE_2D, this->depth_texture );
    if (value)
    {
        if(depth_mode==1)
        value=GlViewCode(value);
        else
            value=GlViewColor(value);

        char * buffer = new char[value->width*value->height*value->nChannels];
        int step     = value->widthStep;
        int height   = value->height;
        int width    = value->width;
        int channels = value->nChannels;
        char * data  = (char *)value->imageData;
        for (int i=0;i<height;i++)
        {
            memcpy(&buffer[i*width*channels],&(data[i*step]),width*channels);
        }

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGB,
            value->width,
            value->height,
            0,
            GL_BGR,
            GL_UNSIGNED_BYTE,
            buffer);
        delete[] buffer;
    }
}

void kinect::UpdateColor(IplImage * value)
{
    glBindTexture( GL_TEXTURE_2D, this->rgb_texture );
    if (value)
    {
        IplImage * temp= cvCloneImage(value);
        cvCvtColor(temp,temp, CV_RGB2BGR);

        char * buffer = new char[temp->width*temp->height*temp->nChannels];
        int step     = temp->widthStep;
        int height   = temp->height;
        int width    = temp->width;
        int channels = temp->nChannels;
        char * data  = (char *)temp->imageData;
        for (int i=0;i<height;i++)
        {
            memcpy(&buffer[i*width*channels],&(data[i*step]),width*channels);
        }

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGB,
            temp->width,
            temp->height,
            0,
            GL_BGR,
            GL_UNSIGNED_BYTE,
            buffer);
        delete[] buffer;
        cvReleaseImage(&temp);
    }
    else
    {
        std::cout << "ERROR" << std::endl;
    }
}




void kinect::Draw()
{
    IplImage * image;
    
   
    if (mode==0)
    {
//        image=freenect_sync_get_depth_cv(0);
        DrawIplImage(GlViewColor(image),Get_x(),Get_y(),1,-1);
    }
    if (mode==1)
    {
 //       image=freenect_sync_get_rgb_cv(0);
        cvCvtColor(image,image,CV_RGB2BGR);
        DrawIplImage(image,Get_x(),Get_y(),1,-1);
    }
       

}



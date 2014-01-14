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

#include "video.h"
#include "tab.h"

Video::Video(int x, int y, int widht, int height, std::string name, Tab* parent)
{
    this->SetName(name);
    this->SetPosition(x, y);
    this->SetSize(widht, height);
    image=NULL;
    bloqueado=false;
    parent->InsertWidget(this);

}

void Video::update()
{

}

void Video::SetImage (IplImage * value)
{
    do {
    } while (bloqueado);
    bloqueado=true;
    cvReleaseImage(&image);
    if (value!=NULL)
    {

        image = cvCloneImage(value);
    }
    bloqueado=false;


}

void DrawIplImage(IplImage *image, int x = 0, int y = 0, GLfloat xZoom = 1.0f, GLfloat yZoom = 1.0f)
{
    GLenum format;
    switch(image->nChannels) {
    case 1:
        format = GL_LUMINANCE;
        break;
    case 2:
        format = GL_LUMINANCE_ALPHA;
        break;
    case 3:
        format = GL_BGR;
        break;
    default:
        return;
    }

    glRasterPos2i(x, y);
    glPixelZoom(xZoom, -yZoom);
    glDrawPixels(image->width, image->height, format, GL_UNSIGNED_BYTE, image->imageData);
    glPixelZoom(xZoom, yZoom);

}

void Video::Draw()
{

    if (image!=NULL)
        DrawIplImage(image, Get_x(), Get_y());

}

void Video::proccesInput()
{

}


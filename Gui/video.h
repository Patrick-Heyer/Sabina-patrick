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


#ifndef VIDEO_H
#define VIDEO_H

#pragma once

#include <list>
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <sstream>
#include <opencv/cv.h>
#include <opencv/highgui.h>


#include "view2d.h"
#include "widget.h"

class video : public widget
{
public:
    video(int x, int y, int widht, int height, std::string name, tab* parent);
    void SetImage( IplImage *value);
    void Draw();
    void Update();

    
private:
       IplImage * image;
};

extern void DrawIplImage(IplImage *image, int x, int y, GLfloat xZoom , GLfloat yZoom );

#endif // VIDEO_H

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


#ifndef KINECT_H
#define KINECT_H

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

class kinect : public widget
{
public:
    kinect(int x, int y, int widht, int height, std::string name, tab* parent);
    void SetMode(int value) {
        mode=value;
    };
    void SetDepthMode(int value) {
        depth_mode=value;
    };
    void UpdateDepth(IplImage * value);
    void UpdateColor(IplImage * value);
    void Draw();
    void Update();
    IplImage* freenect_sync_get_depth_cv(int arg1);


private:
    int mode;
    int depth_mode;
    IplImage * depth;
    IplImage * color;
    GLuint depth_texture;
    GLuint rgb_texture;
};



#endif // KINECT_H

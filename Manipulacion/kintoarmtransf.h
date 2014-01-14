/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2013  <copyright holder> <email>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef KINTOARMTRANSF_H
#define KINTOARMTRANSF_H

#include<mrpt/base.h>

class KinToArmTransf
{

public:
KinToArmTransf();

   /**
   * Stabish the pose parameters of the Kinect with respect of the arm
   * Parameters must be given in meters and radians
   */
  void setKinectPosition(float x, float y, float z, float th);
  
  /**
   * Transform a point seen by kinect to the arm reference frame
   */
  void tranformToArm(float x, float y, float z, float &x_arm, float &y_arm, float &z_arm) const;
  
protected:
  float cx, cy, cz;
  float theta;
};

#endif // KINTOARMTRANSF_H

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


#include "kintoarmtransf.h"

KinToArmTransf::KinToArmTransf()
{

}

void KinToArmTransf::tranformToArm(float x, float y, float z, float& x_arm, float& y_arm, float& z_arm) const
{
  mrpt::math::CMatrixFloat44 R;
  mrpt::math::CArrayFloat<4> P, P_prima;
  
  //homogenous transformation matrix
  R(0,0) = 0;
  R(1,0) = -1;
  R(2,0) = 0;
  R(3,0) = 0;
  
  R(0,1) = sin(theta);
  R(1,1) = 0;
  R(2,1) = -cos(theta);
  R(3,1) = 0;
  
  R(0,2) = cos(theta);
  R(1,2) = 0;
  R(2,2) = sin(theta);
  R(3,2) = 0;
  
  R(0,3) = cx;
  R(1,3) = cy;
  R(2,3) = cz;
  R(3,3) = 1;
  
  // vector
  P(0,0) = x;
  P(1,0) = y;
  P(2,0) = z;
  P(3,0) = 1;
  
  std::cout << "R " << std::endl << R << std::endl;
  
  P_prima = R * P;
  
  std::cout << "Converted point" << P_prima << std::endl;
  x_arm = P_prima(0,0);
  y_arm = P_prima(1,0);
  z_arm = P_prima(2,0);
}


void KinToArmTransf::setKinectPosition(float x, float y, float z, float th)
{
  cx = x;
  cy = y;
  cz = z;
  theta = th;
}

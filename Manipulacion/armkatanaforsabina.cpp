/*
    <one line to give the program's name and a brief idea of what it does.>
    2012  J. Irving Vasquez  ivasquez@ccc.inaoep.mx

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


#include "armkatanaforsabina.h"


ArmKatanaForSabina::ArmKatanaForSabina(): ArmKatana()
{

  // carriying position
  poseCarriying.resize(6);
  poseCarriying[0] = 360.0;
  poseCarriying[1] = 0.0;
  poseCarriying[2] = 170.0;
  poseCarriying[3] = 1.57;
  poseCarriying[4] = 1.57;
  poseCarriying[5] = 0.0;
  
  encNavigationPosition.resize(6);
  encNavigationPosition[0] =  6346;
  encNavigationPosition[1] = -30991;
  encNavigationPosition[2] = -30995;
  encNavigationPosition[3] = 6669;
  encNavigationPosition[4] = 6609;
  encNavigationPosition[5] = 30770; 
}


void ArmKatanaForSabina::moveToNavigationPos()
{
  katana->moveRobotToEnc(encNavigationPosition.begin(), encNavigationPosition.end(), true);
}

void ArmKatanaForSabina::moveToCarriyingPos()
{
  katana->moveRobotTo(poseCarriying, true);
}


int ArmKatanaForSabina::graspObjectAt(double x, double y, double z)
{
    std::cout << "Grasping object..." << std::endl;
  double phi;
  double theta;
  double psi = 0;
  
  double phi_a;
  double theta_a;
  double psi_a = 0;  
  double x_a, y_a, z_a;

  //compute approaching pose

  computeApproachingPos(x,y,z,x_a,y_a,z_a);
  computeOrientation(x_a,y_a,z_a,phi_a, theta_a);
  
  katana->moveRobotTo(x_a, y_a, z_a, phi_a, theta_a, psi_a, true);
  katana->openGripper(true);
  sleep(1);
  
  // move to grasping pose

  computeOrientation(x, y, z, phi, theta);

  katana->moveRobotTo(x, y, z, phi, theta, psi); 
  //sleep(0.5);
  katana->closeGripper(false);	
  sleep(3);
  std::cout << "Grasping position reached." << std::endl; 
  
  moveToCarriyingPos();
  std::cout << "Carriying position reached." << std::endl; 
  
  return 0;
}

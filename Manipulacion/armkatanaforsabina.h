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


#ifndef ARMKATANAFORSABINA_H
#define ARMKATANAFORSABINA_H

#include "armkatana.h"
//#include "vpfilereader.h"
#include <ctime>
 
using namespace std;

/**
 * Object to work with markovito.
 * 
 * Capabilities:
 * 	-	Grasp a object.
 * 	- 	Movement to simple poses.
 * 
 * Requirements:
 * 	- 	The object to grasp must be inside the reachable space.
 * 	-	Vertical position of the arm.
 * 
 * Notes:	
 * 	-	There is no collision checking. So the provided positions must be tested before.
 */
class ArmKatanaForSabina : public ArmKatana
{

public:
ArmKatanaForSabina();

  /**
   * x,y,z are in arm coordinates. milimeters
   */
  int testGrasping(double x, double y, double z);
  
  /**
   * x,y,z are in arm coordinates. milimeters
   */
  int Grasping(double x, double y, double z);

  /**
   * 
   */
  int graspObjectAtKinectImage(double x, double y);
  
  void moveToCarriyingPos();
  
  void deliverObject();
  
  void store(); 
  
  void bringOut();
  
  void moveToHanging();


protected:
  bool equalVectors(vector<int> A, vector<int> B, int tolerance);
};

#endif // ARMKATANAFORSABINA_H

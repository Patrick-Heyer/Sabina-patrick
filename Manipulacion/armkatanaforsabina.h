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
 
using namespace std;

class ArmKatanaForSabina : public ArmKatana
{

public:
ArmKatanaForSabina();
//virtual ~ArmKatanaForSabina();

int graspObjectAt(double x,  double y, double z);

void setPoseCarriying(double x,  double y, double z, double phi, double theta, double psi);

//void setPoseGaspingSheet(double x,  double y, double z, double phi, double theta, double psi);

void moveToNavigationPos();

void moveToCarriyingPos();


private:
    std::vector<double> poseCarriying;
    std::vector<double> poseGraspingSheet;
    std::vector<int> encNavigationPosition;
    //std::vector<int> encCarriyingPosition;
};

#endif // ARMKATANAFORSABINA_H

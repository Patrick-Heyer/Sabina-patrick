#include <iostream>

#include "Laser.h"

bool laser_escribible=true;

Laser::Laser()
{
    laserReadings.clear();
}
void Laser::setLaserScan(std::vector< float > data)
{
    //laserReadings.clear();
    do
    {

    } while(!laser_escribible);
// 	std::cout << "estoy escribiendo" << std::endl;
    laser_escribible=false;
    laserReadings= data;


    laser_escribible=true;
}
std::vector< float > Laser::getLaserScan()
{

// 	std::vector<float> tempLaser;
// 	do
// 	{
//
// 	}while(!laser_escribible);
// 	laser_escribible=false;
// // 	std::cout << "pude leer" << std::endl;
// 	tempLaser.clear();
// 	tempLaser =laserReadings;
// 	laser_escribible=true;
// 	return tempLaser;

    return laserReadings;
}






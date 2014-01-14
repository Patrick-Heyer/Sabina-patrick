
#include "Destination.h"

bool Name_escribible=true;
bool Coordinate_escribible=true;

///*! \brief Gets the Location of the destination
// *
// *  Gets the Location of the destination \return Coordinate
// */
Location Destination::get_Coordinate() {
  return Coordinate;
}

///*! \brief Sets the Location of the destination
// *
// *  Sets the Location of the destination
// */
void Destination::set_Coordinate(Location value) {
  do
  {
  }while (!Coordinate_escribible);
  Coordinate_escribible=false;
  Coordinate = value;
  Coordinate_escribible=true;
}

///*! \brief Gets the name of the destination
// *
// *  Gets the name of the destination \return Name
// */
string Destination::get_Name() {
  return *Name;
}

///*! \brief Sets the name of the destination
// *
// *  Sets the name of the destination
// */
void Destination::set_Name(string value) {
    do
    {
    }while (!Name_escribible);
    Name_escribible=false;
    *Name = value;
    Name_escribible=true;
}



#include "Destination.h"

bool Name_dest_escribible=true;
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
    } while (!Coordinate_escribible);
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
    } while (!Name_dest_escribible);
    Name_dest_escribible=false;
    Name=new string;
    *Name = value;
    Name_dest_escribible=true;
}


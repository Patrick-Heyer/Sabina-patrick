#ifndef _DESTINATION_H
#define _DESTINATION_H


#include <string>
using namespace std;
#include "Location.h"

///*! \brief Associates a name to a Location
// *
// *   Associates a name to a Location so it can be referenced by name
// */
class Destination {
private:
    ///*! \brief The name of the destination
    // *
    // *
    // */
    string * Name;

    ///*! \brief The Location of the destination
    // *
    // *
    // */
    Location Coordinate;


public:
    ///*! \brief Gets the Location of the destination
    // *
    // *  Gets the Location of the destination \return Coordinate
    // */
    Location get_Coordinate();

    ///*! \brief Sets the Location of the destination
    // *
    // *  Sets the Location of the destination
    // */
    void set_Coordinate(Location value);

    ///*! \brief Gets the name of the destination
    // *
    // *  Gets the name of the destination \return Name
    // */
    string get_Name();

    ///*! \brief Sets the name of the destination
    // *
    // *  Sets the name of the destination
    // */
    void set_Name(string value);

    friend class Robot;
};
#endif

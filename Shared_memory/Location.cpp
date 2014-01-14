
#include "Location.h"

bool X_escribible=true;
bool Y_escribible=true;
bool Angle_escribible=true;

///*! \brief Gets the X position of the location
// *
// *  Gets the X position of the location \return X
// */
int Location::get_X() {
    return this->X;
}

///*! \brief Sets the X position of the location
// *
// *  Sets the X position of the location
// */
void Location::set_X(int value) {
    do
    {
    } while (!X_escribible);
    X_escribible=false;
    this->X = value;
    X_escribible=true;
}

///*! \brief Gets the Y position of the location
// *
// *  Gets the Y position of the location \return Y
// */
int Location::get_Y() {
    return Y;
}

///*! \brief Sets the Y position of the location
// *
// *  Sets the Y position of the location
// */
void Location::set_Y(int value) {
    do
    {
    } while (!Y_escribible);
    Y_escribible=false;
    Y = value;
    Y_escribible=true;
}

///*! \brief Gets the Angle of the location
// *
// *  Gets the Angle of the location \return Angle
// */
float Location::get_Angle() {
    return Angle;
}

///*! \brief Sets the Angle of the location
// *
// *  Sets the Angle of the location
// */
void Location::set_Angle(float value) {
    do
    {
    } while (!Angle_escribible);
    Angle_escribible=false;
    Angle = value;
    Angle_escribible=true;
}

///*! \brief Sets the X, Y positions and the Angle of the location
// *
// *  Sets the X, Y positions and the Angle of the location
// */
void Location::set(int x, int y, float angle) {
    this->set_X(x);
    this->set_Y(y);
    this->set_Angle(angle);
}


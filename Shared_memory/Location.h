#ifndef _LOCATION_H
#define _LOCATION_H


///*! \brief Contains the info about a position in 2D space
// *
// *  Contains the info about a position in 2D space x, y , angle it is used to
// *  define points on the map
// */
class Location {
private:
    ///*! \brief The X position of the location
    // *
    // *
    // */
    int X;

    ///*! \brief The Y position of the location
    // *
    // *
    // */
    int Y;

    ///*! \brief The angle of the location
    // *
    // *
    // */
    float Angle;


public:
    ///*! \brief Gets the X position of the location
    // *
    // *  Gets the X position of the location \return X
    // */
    int get_X();

    ///*! \brief Sets the X position of the location
    // *
    // *  Sets the X position of the location
    // */
    void set_X(int value);

    ///*! \brief Gets the Y position of the location
    // *
    // *  Gets the Y position of the location \return Y
    // */
    int get_Y();

    ///*! \brief Sets the Y position of the location
    // *
    // *  Sets the Y position of the location
    // */
    void set_Y(int value);

    ///*! \brief Gets the Angle of the location
    // *
    // *  Gets the Angle of the location \return Angle
    // */
    float get_Angle();

    ///*! \brief Sets the Angle of the location
    // *
    // *  Sets the Angle of the location
    // */
    void set_Angle(float value);

    ///*! \brief Sets the X, Y positions and the Angle of the location
    // *
    // *  Sets the X, Y positions and the Angle of the location
    // */
    void set(int x, int y, float angle);

    friend class Robot;
};
#endif

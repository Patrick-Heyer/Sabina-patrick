#ifndef _OBJECTIVE_H
#define _OBJECTIVE_H


#include <string>
using namespace std;

///*! \brief Contains a mixture of information
// *
// *  Contains a mixture of information about objects or persons the robot
// *  has seen but that where irelevant at that moment but could be usefull
// *  in the future
// */
class Objective {
private:
    ///*! \brief Name of the Objective
    // *
    // *
    // */
    string * Name;

    ///*! \brief Visual_ID of the Objective
    // *
    // *  Visual_ID of the Objective given by the vision system
    // */
    int Visual_ID;

    ///*! \brief Type of the Objective
    // *
    // *  Type of the Objective person object or any new type
    // */
    string * Type;

    ///*! \brief Last X position the Objective was seen
    // *
    // *
    // */
    float Last_x;

    ///*! \brief Last Y position the Objective was seen
    // *
    // *
    // */
    float Last_y;


public:
  Objective();
    ///*! \brief Gets the Visual_ID of the objective
    // *
    // *  Gets the Visual_ID of the objective \return Visual_ID
    // */
    int get_Visual_ID();

    ///*! \brief Sets the visual_ID of the objective
    // *
    // *  Sets the visual_ID of the objective
    // */
    void set_Visual_ID(int value);

    ///*! \brief Gets the type of the objective
    // *
    // *  Gets the type of the objective person, object \return Type
    // */
    string * get_Type();

    ///*! \brief Sets the type of the objective
    // *
    // *  Sets the type of the objective person, object
    // */
    void set_Type(string  value);

    ///*! \brief Gets the last_x of the objective
    // *
    // *  Gets the last X position of the objective \return Last_x
    // */
    float get_Last_x();

    ///*! \brief Sets the last_x of the objective
    // *
    // *  Sets the last X position of the objective
    // */
    void set_Last_x(float value);

    ///*! \brief Gets the last_y of the objective
    // *
    // *  Gets the last Y position of the objective \return Last_y
    // */
    float get_Last_y();

    ///*! \brief Sets the last_y of the objective
    // *
    // *  Sets the last Y position of the objective
    // */
    void set_Last_y(float value);

    ///*! \brief Gets the name of the objective
    // *
    // *  Gets the name of the objective \return Name
    // */
    string get_Name();

    ///*! \brief Sets the name of the objective
    // *
    // *  Sets the name of the objective
    // */
    void set_Name(string value);

    friend class Robot;
};
#endif

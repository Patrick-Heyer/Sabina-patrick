
#include "Objective.h"

bool Name_escribible=true;
bool Visual_ID_obj__escribible=true;
bool Type_escribible=true;
bool Last_x_escribible=true;
bool Last_y_escribible=true;

///*! \brief Gets the Visual_ID of the objective
// *
// *  Gets the Visual_ID of the objective \return Visual_ID
// */
int Objective::get_Visual_ID() {
    return Visual_ID;
}

///*! \brief Sets the visual_ID of the objective
// *
// *  Sets the visual_ID of the objective
// */
void Objective::set_Visual_ID(int value) {
    do
    {
    } while (!Visual_ID_obj__escribible);
    Visual_ID_obj__escribible=false;
    Visual_ID = value;
    Visual_ID_obj__escribible=true;
}

///*! \brief Gets the type of the objective
// *
// *  Gets the type of the objective person, object \return Type
// */
string * Objective::get_Type() {
    return Type;
}

///*! \brief Sets the type of the objective
// *
// *  Sets the type of the objective person, object
// */
void Objective::set_Type(string value) {
    if (Type==NULL) Type=new string;
    *Type = value;
}

///*! \brief Gets the last_x of the objective
// *
// *  Gets the last X position of the objective \return Last_x
// */
float Objective::get_Last_x() {
    return Last_x;
}

///*! \brief Sets the last_x of the objective
// *
// *  Sets the last X position of the objective
// */
void Objective::set_Last_x(float value) {
    do
    {
    } while (!Last_x_escribible);
    Last_x_escribible=false;
    Last_x = value;
    Last_x_escribible=true;
}

///*! \brief Gets the last_y of the objective
// *
// *  Gets the last Y position of the objective \return Last_y
// */
float Objective::get_Last_y() {
    return Last_y;
}

///*! \brief Sets the last_y of the objective
// *
// *  Sets the last Y position of the objective
// */
void Objective::set_Last_y(float value) {
    do
    {
    } while (!Last_y_escribible);
    Last_y_escribible=false;
    Last_y = value;
    Last_y_escribible=true;
}

///*! \brief Gets the name of the objective
// *
// *  Gets the name of the objective \return Name
// */
string Objective::get_Name() {
    return *Name;
}

///*! \brief Sets the name of the objective
// *
// *  Sets the name of the objective
// */
void Objective::set_Name(string value) {
    do
    {
    } while (!Name_escribible);
    Name_escribible=false;
    if (Name==NULL) Name=new string;
    *Name = value;
    Name_escribible=true;
}


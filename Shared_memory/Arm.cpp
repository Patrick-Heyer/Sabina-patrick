
#include "Arm.h"

bool State_escribible=true;
bool Gripper_x_escribible=true;
bool Gripper_y_escribible=true;
bool Gripper_z_escribible=true;
bool Gripper_open_escribible=true;

///*! \brief Gets the state of the arm
// *
// *  Gets the state of the arm \return State
// */
int Arm::getState() {
    return state;
}

///*! \brief Sets the state of the arm
// *
// *  Sets the state of the arm
// */
void Arm::setState(int value) {
}

///*! \brief Gets the X coordinate of the end effector
// *
// *  Gets the X coordinate of the end effector \return Gripper_x
// */
int Arm::getGripperX() {
    return gripperX;
}

///*! \brief Sets the X coordinate of the end effector
// *
// *  Sets the X coordinate of the end effector
// */
void Arm::setGripperX(int value) {
    gripperX = value;
}

///*! \brief Gets the Y coordinate of the end effector
// *
// *  Gets the Y coordinate of the end effector \return Gripper_y
// */
int Arm::getGripperY() {
    return gripperY;
}

///*! \brief Sets the Y coordinate of the end effector
// *
// *  Sets the Y coordinate of the end effector
// */
void Arm::setGripperY(int value) {
    gripperY = value;
}

///*! \brief Gets the Z coordinate of the end effector
// *
// *  Gets the Z coordinate of the end effector \return Gripper_z
// */
int Arm::getGripperZ() {
    return gripperZ;
}

///*! \brief Sets the Z coordinate of the end effector
// *
// *  Sets the Z coordinate of the end effector
// */
void Arm::setGripperZ(int value) {
    gripperZ = value;
}

///*! \brief Gets the state of the end effector
// *
// *  Gets the state of the end effector \return Gripper_open
// */
bool Arm::getGripperOpen() {
    return gripperOpen;
}

///*! \brief Sets the state of the end effector
// *
// *  Sets the state of the end effector
// */
void Arm::setGripperOpen(bool value) {
    gripperOpen = value;
}


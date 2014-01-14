
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
int Arm::get_State() {
  return State;
}

///*! \brief Sets the state of the arm
// *
// *  Sets the state of the arm
// */
void Arm::set_State(int value) {
}

///*! \brief Gets the X coordinate of the end effector
// *
// *  Gets the X coordinate of the end effector \return Gripper_x
// */
int Arm::get_Gripper_x() {
  return Gripper_x;
}

///*! \brief Sets the X coordinate of the end effector
// *
// *  Sets the X coordinate of the end effector
// */
void Arm::set_Gripper_x(int value) {
  Gripper_x = value;
}

///*! \brief Gets the Y coordinate of the end effector
// *
// *  Gets the Y coordinate of the end effector \return Gripper_y
// */
int Arm::get_Gripper_y() {
  return Gripper_y;
}

///*! \brief Sets the Y coordinate of the end effector
// *
// *  Sets the Y coordinate of the end effector
// */
void Arm::set_Gripper_y(int value) {
  Gripper_y = value;
}

///*! \brief Gets the Z coordinate of the end effector
// *
// *  Gets the Z coordinate of the end effector \return Gripper_z
// */
int Arm::get_Gripper_z() {
  return Gripper_z;
}

///*! \brief Sets the Z coordinate of the end effector
// *
// *  Sets the Z coordinate of the end effector
// */
void Arm::set_Gripper_z(int value) {
  Gripper_z = value;
}

///*! \brief Gets the state of the end effector
// *
// *  Gets the state of the end effector \return Gripper_open
// */
bool Arm::get_Gripper_open() {
  return Gripper_open;
}

///*! \brief Sets the state of the end effector
// *
// *  Sets the state of the end effector
// */
void Arm::set_Gripper_open(bool value) {
  Gripper_open = value;
}


#ifndef _ARM_H
#define _ARM_H


///*! \brief Information about the information of the arm
// *
// *  
// */
class Arm {
  private:
    ///*! \brief Defines the state of the arm
    // *
    // *   Define the state of the arm save, extended, grabing position 
    // */
    int State;

    ///*! \brief X position of the end effector
    // *
    // *  
    // */
    int Gripper_x;

    ///*! \brief Y position of the end effector
    // *
    // *  
    // */
    int Gripper_y;

    ///*! \brief Z position of the end effector
    // *
    // *  
    // */
    int Gripper_z;

    ///*! \brief State of the end effector
    // *
    // *  State of the end effector open true close false
    // */
    bool Gripper_open;


  public:
    ///*! \brief Gets the state of the arm
    // *
    // *  Gets the state of the arm \return State
    // */
    int get_State();

    ///*! \brief Sets the state of the arm
    // *
    // *  Sets the state of the arm
    // */
    void set_State(int value);

    ///*! \brief Gets the X coordinate of the end effector
    // *
    // *  Gets the X coordinate of the end effector \return Gripper_x
    // */
    int get_Gripper_x();

    ///*! \brief Sets the X coordinate of the end effector
    // *
    // *  Sets the X coordinate of the end effector
    // */
    void set_Gripper_x(int value);

    ///*! \brief Gets the Y coordinate of the end effector
    // *
    // *  Gets the Y coordinate of the end effector \return Gripper_y
    // */
    int get_Gripper_y();

    ///*! \brief Sets the Y coordinate of the end effector
    // *
    // *  Sets the Y coordinate of the end effector
    // */
    void set_Gripper_y(int value);

    ///*! \brief Gets the Z coordinate of the end effector
    // *
    // *  Gets the Z coordinate of the end effector \return Gripper_z
    // */
    int get_Gripper_z();

    ///*! \brief Sets the Z coordinate of the end effector
    // *
    // *  Sets the Z coordinate of the end effector
    // */
    void set_Gripper_z(int value);

    ///*! \brief Gets the state of the end effector
    // *
    // *  Gets the state of the end effector \return Gripper_open
    // */
    bool get_Gripper_open();

    ///*! \brief Sets the state of the end effector
    // *
    // *  Sets the state of the end effector
    // */
    void set_Gripper_open(bool value);

  friend class Robot;
};
#endif

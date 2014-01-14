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
    // *   Define the state of the arm safe, extended, grabing position
    // */
    int state;

    ///*! \brief X position of the end effector
    // *
    // *
    // */
    int gripperX;

    ///*! \brief Y position of the end effector
    // *
    // *
    // */
    int gripperY;

    ///*! \brief Z position of the end effector
    // *
    // *
    // */
    int gripperZ;

    ///*! \brief State of the end effector
    // *
    // *  State of the end effector open true close false
    // */
    bool gripperOpen;


public:
    ///*! \brief Gets the state of the arm
    // *
    // *  Gets the state of the arm \return State
    // */
    int getState();

    ///*! \brief Sets the state of the arm
    // *
    // *  Sets the state of the arm
    // */
    void setState(int value);

    ///*! \brief Gets the X coordinate of the end effector
    // *
    // *  Gets the X coordinate of the end effector \return Gripper_x
    // */
    int getGripperX();

    ///*! \brief Sets the X coordinate of the end effector
    // *
    // *  Sets the X coordinate of the end effector
    // */
    void setGripperX(int value);

    ///*! \brief Gets the Y coordinate of the end effector
    // *
    // *  Gets the Y coordinate of the end effector \return Gripper_y
    // */
    int getGripperY();

    ///*! \brief Sets the Y coordinate of the end effector
    // *
    // *  Sets the Y coordinate of the end effector
    // */
    void setGripperY(int value);

    ///*! \brief Gets the Z coordinate of the end effector
    // *
    // *  Gets the Z coordinate of the end effector \return Gripper_z
    // */
    int getGripperZ();

    ///*! \brief Sets the Z coordinate of the end effector
    // *
    // *  Sets the Z coordinate of the end effector
    // */
    void setGripperZ(int value);

    ///*! \brief Gets the state of the end effector
    // *
    // *  Gets the state of the end effector \return Gripper_open
    // */
    bool getGripperOpen();

    ///*! \brief Sets the state of the end effector
    // *
    // *  Sets the state of the end effector
    // */
    void setGripperOpen(bool value);

    friend class Robot;
};
#endif

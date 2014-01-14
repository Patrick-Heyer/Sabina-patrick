
#include "Robot.h"
#include "Destination.h"
#include "Objective.h"

#include "ConfigFile.h"
#include "Triplet.h"

bool Main_system_escribible=true;
bool Position_escribible=true;
bool Lineal_velocity_escribible=true;
bool Angular_velocity_escribible=true;
bool Action_escribible=true;
bool Last_objective_escribible=true;
bool Current_destination_escribible=true;
bool Route_escribible=true;

using std::string;
using std::cout;
using std::endl;

Robot * Robot::m_singleton=0;

///*! \brief Private constructor 
// *
// *  The constructor has to be private or protected so the singleton patern works
// */
Robot::Robot(){
}

///*! \brief Starts the singleton
// *
// *  Starts the singleton similar to a constructor. Its the only way to set initial values
// *  since the constructor is private. The initial values are read from the a configuration
// *  file given by filename
// */
void Robot::Initialize(string filename) {
    if ( m_singleton == 0 ) 
          get_Instance();
  
    ConfigFile config( filename );
    
    int temp_int;
    float temp_float;
    string temp_string;
    config.readInto( get_Instance().Main_system, "Main_system" );
    config.readInto( temp_int, "Position_x" );
    get_Instance().Position.set_X(temp_int);
    config.readInto( temp_int, "Position_y" );
    get_Instance().Position.set_Y(temp_int);
    config.readInto( temp_float, "Position_angle" );
    get_Instance().Position.set_Angle(temp_float);
    config.readInto( get_Instance().Lineal_velocity, "Lineal_velocity" );
    config.readInto( get_Instance().Angular_velocity, "Angular_velocity" );
  
    get_Instance().Action = new string;
    config.readInto( temp_string, "Action" );
    get_Instance().set_Action(temp_string);
  
    get_Instance().Last_objective = new string;
    config.readInto( temp_string, "Last_objective" );
    get_Instance().set_Last_objective(temp_string);
  
    get_Instance().Current_destination = new string;
    config.readInto( temp_string, "Current_destination" );
    get_Instance().set_Current_destination(temp_string);
  
    get_Instance().Sintetizer.Phrase = new string;

    config.readInto( temp_int, "Camera_index" );
    get_Instance().stream=cvCreateCameraCapture (temp_int);
    get_Instance().get_image();
   //
}

IplImage * Robot::get_image()
{

   return cvQueryFrame(get_Instance().stream);
    
}


///*! \brief Returns a pointer to the Robot object
// *
// *  Returns a pointer to the only copy in memory of the Robot object. If no object exist it
// *  will initialize the instance befor returning the pointer \return Robot
// */
Robot & Robot::get_Instance() {
        if ( m_singleton == 0 )
        {
            m_singleton = new Robot;

        }
    
        return ((Robot &)m_singleton);
}

///*! \brief Gets the position of the robot
// *
// *  Gets the position of the robot (x, y, angle) \return Position
// */
Location Robot::get_Position() {
  return Position;
}

///*! \brief Sets the position of the robot
// *
// *  Sets the position of the robot (x, y, angle)
// */
void Robot::set_Position(Location value) {
  {
  }while (!Position_escribible);
  Position_escribible=false;
  Position = value;
  Position_escribible=true;
}

///*! \brief Gets the Lineal velocity of the robot
// *
// *  Gets the Lineal velocity of the robot \return Lineal_velocity
// */
float Robot::get_Lineal_velocity() {
  return Lineal_velocity;
}

///*! \brief Sets the Lineal velocity of the robot
// *
// *  Sets the Lineal velocity of the robot 
// */
void Robot::set_Lineal_velocity(float value) {
  do
  {
  }while (!Lineal_velocity_escribible);
  Lineal_velocity_escribible=false;
  Lineal_velocity = value;
  Lineal_velocity_escribible=true;
}

///*! \brief Gets the Angular velocity of the robot
// *
// *  Gets the Angular velocity of the robot \return Angular_velocity
// */
float Robot::get_Angular_velocity() {
  return Angular_velocity;
}

///*! \brief Sets the Angular velocity of the robot
// *
// *  Sets the Angular velocity of the robot 
// */
void Robot::set_Angular_velocity(float value) {
  do
  {
  }while (!Angular_velocity_escribible);
  Angular_velocity_escribible=false;
  Angular_velocity = value;
  Angular_velocity_escribible=true;
}

///*! \brief Gets the name of the destination
// *
// *  Gets the name of the destination of the robot. There is no safty check to see if the
// *  destination exists \return Current_destination
// */
string Robot::get_Current_destination() {
  return *Current_destination;
}

///*! \brief Sets the name of the destination
// *
// *  Sets the name of the destination of the robot. There is no safty check to see if the
// *  destination exists
// */
void Robot::set_Current_destination(string value) {
    do
    {
    }while (!Current_destination_escribible);
    Current_destination_escribible=false;
    *Current_destination = value;
    Current_destination_escribible=true;
}

///*! \brief Gets the name of last seen of the last seen object
// *
// *  Gets the name of last seen of the last seen object. There is no safty check to 
// *  see if the name of the object exists \return Last_objective
// */
string Robot::get_Last_objective() {
  return *Last_objective;
}

///*! \brief Sets the name of last seen of the last seen object
// *
// *  Sets the name of last seen of the last seen object. There is no safty check to 
// *  see if the name of the object exists.
// */
void Robot::set_Last_objective(string value) {
    do
    {
    }while (!Last_objective_escribible);
    Last_objective_escribible=false;
    *Last_objective = value;
    Last_objective_escribible=true;
}

///*! \brief Gets the route of the robot
// *
// *  Gets a list of Locations that form the route or path  \return Route
// */
list<Location> & Robot::get_Route() {
  return Route;
}

///*! \brief Sets the route of the robot
// *
// *  Sets a list of Locations that form the route or path.
// */
void Robot::set_Route(list<Location> & value) {
  do
  {
  }while (!Route_escribible);
  Route_escribible=false;
  Route = value;
  Route_escribible=true;
}

///*! \brief Gets the name of the action decided by the MDP
// *
// *   Gets the name of the action decided by the MDP \return Action
// */
string Robot::get_Action() {
  return *Action;
}

///*! \brief Sets the name of the action decided by the MDP
// *
// *   Sets the name of the action decided by the MDP 
// */
void Robot::set_Action(string value) {
    do
    {
    }while (!Action_escribible);
    Action_escribible=false;
    *Action = value;
    Action_escribible=true;
}

///*! \brief Adds a location to the Route
// *
// *  Adds a location to the end of the list of locations containd in Route
// */
void Robot::Add_Location_to_path(const Location & location) {
}

///*! \brief Loads the list of known locations from file
// *
// *  Loads the list of known locations from file \return false if file not found
// */
bool Robot::Load_Destinations(string filename) {
}

///*! \brief Deletes a location to the Route
// *
// *  Deletes the first element of the Route list
// */
void Robot::Delete_Location_from_path() {
}

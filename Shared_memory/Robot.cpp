#include <iostream>
#include <fstream>
#include <string>

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
bool Aria_port_escribible=true;
bool Laser_port_escribible=true;
bool PTZ_port_escribible=true;
bool Last_objective_escribible=true;
bool Current_destination_escribible=true;
bool Prev_destination_escribible=true;
bool Route_escribible=true;
bool estimatedPosition_escribible = true;

using std::string;
using std::cout;
using std::endl;

Robot * Robot::m_singleton=0;

///*! \brief Private constructor
// *
// *  The constructor has to be private or protected so the singleton patern works
// */
Robot::Robot() {
}


void load_destinations ( char *filename, map<string,Destination> &mine)
{
    string name;
    int x, y, a;
    Location loc_temp;
    Destination temp;
    ifstream myfile (filename);
    if (myfile.is_open())
    {
        while ( myfile.good() )
        {
            myfile >> name;
            myfile >> x;
            myfile >> y;
            myfile >> a;
            loc_temp.set(x,y,a);
            temp.set_Coordinate(loc_temp);

            temp.set_Name(name);
            mine.insert(std::make_pair( name,temp));
            cout <<temp.get_Name() << temp.get_Coordinate().get_X() <<endl;
        }
        myfile.close();
    }
}

void load_Names ( char *filename, map<string,Objective> &mine)
{
    string name;
    Objective temp;

    ifstream myfile (filename);
    if (myfile.is_open())
    {
        while ( myfile.good() )
        {
            myfile >> name;

            mine.insert(std::make_pair( name,temp));
        }
        myfile.close();
    }
}

///*! \brief Starts the singleton
// *
// *  Starts the singleton similar to a constructor. Its the only way to set initial values
// *  since the constructor is private. The initial values are read from the a configuration
// *  file given by filename
// */
void Robot::Initialize(string filename) {
    if ( m_singleton == 0 )
        getInstance();

    ConfigFile config( filename );
    

    int temp_int;
    float temp_float;
    string temp_string;
    config.readInto( getInstance().Main_system, "Main_system" );

    config.readInto( temp_int, "Position_x" );
    getInstance().Position.set_X(temp_int);

    config.readInto( temp_int, "Position_y" );
    getInstance().Position.set_Y(temp_int);

    config.readInto( temp_float, "Position_angle" );
    getInstance().Position.set_Angle(temp_float);



    config.readInto( getInstance().Lineal_velocity, "Lineal_velocity" );
    config.readInto( getInstance().Angular_velocity, "Angular_velocity" );

    getInstance().Aria_port = new string;
    config.readInto( temp_string, "Aria_port" );
    getInstance().set_Aria_port(temp_string);

    getInstance().Laser_port = new string;
    config.readInto( temp_string, "Laser_port" );
    getInstance().set_Laser_port(temp_string);

    getInstance().PTZ_port = new string;
    config.readInto( temp_string, "PTZ_port" );
    getInstance().set_PTZ_port(temp_string);

    config.readInto( temp_int, "Camera_index" );
    if (temp_int>=0)
        getInstance().Ptz.set_stream(cvCaptureFromCAM(temp_int));

    getInstance().Enviroment.Image_path = new string;
    config.readInto( temp_string, "Enviroment_Image_path" );
    getInstance().Enviroment.set_Image_path(temp_string);

    getInstance().Destinations=new map<string, Destination>;
    config.readInto(temp_string, "Destinations_file" );
    load_destinations((char*) temp_string.c_str(), *getInstance().Destinations);

    getInstance().Objectives=new map<string, Objective>;
    config.readInto(temp_string, "Names_file" );
    load_Names((char*) temp_string.c_str(),*getInstance().Objectives);

    getInstance().Action = new string;
    getInstance().Last_objective = new string;
    getInstance().Current_destination = new string;
    getInstance().prev_destination= new string;
    getInstance().Sintetizer.Phrase = new string;
    getInstance().Microphone.Phrase=new string;
	getInstance().Microphone.Words=new string;
    getInstance().Route=new list<Location>;
	
	getInstance().seguir=false;
	
	
	
	getInstance().laser= new Laser();
	getInstance().localized = false;
	getInstance().estimatedPosition.set_X(0);
	getInstance().estimatedPosition.set_Y(0);
	getInstance().estimatedPosition.set_Angle(0);
    
}



///*! \brief Returns a pointer to the Robot object
// *
// *  Returns a pointer to the only copy in memory of the Robot object. If no object exist it
// *  will initialize the instance befor returning the pointer \return Robot
// */
Robot & Robot::getInstance() {
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
void Robot::setPosition(Location value) {
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
    } while (!Lineal_velocity_escribible);
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
    } while (!Angular_velocity_escribible);
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
    } while (!Current_destination_escribible);
    Current_destination_escribible=false;
    *Current_destination = value;
    Current_destination_escribible=true;
}

void Robot::set_prev_destination(string value) {
    do
    {
    } while (!Prev_destination_escribible);
    Prev_destination_escribible=false;
    *prev_destination = value;
    Prev_destination_escribible=true;
}

///*! \brief Gets the name of the last seen object
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
    } while (!Last_objective_escribible);
    Last_objective_escribible=false;
    *Last_objective = value;
    Last_objective_escribible=true;
}

///*! \brief Gets the route of the robot
// *
// *  Gets a list of Locations that form the route or path  \return Route
// */
list<Location> & Robot::get_Route() {
    return *Route;
}

///*! \brief Sets the route of the robot
// *
// *  Sets a list of Locations that form the route or path.
// */
void Robot::set_Route(list<Location>  value) {
    do
    {
    } while (!Route_escribible);
    Route_escribible=false;
    *Route = value;
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
    } while (!Action_escribible);
    Action_escribible=false;
    *Action = value;
    Action_escribible=true;
}

///*! \brief Adds a location to the Route
// *
// *  Adds a location to the end of the list of locations containd in Route
// */
void Robot::Add_Location_to_path(const Location & location) {
    Route->push_back(location);
}

///*! \brief Loads the list of known locations from file
// *
// *  Loads the list of known locations from file \return false if file not found
// */
bool Robot::Load_Destinations(string filename) {
}

///*! \brief Deletes a location from the Route
// *
// *  Deletes the first element of the Route list
// */
void Robot::Delete_Location_from_path() {
  Route->pop_front();
}

void Robot::Clear_path()
{
	Route->clear();
}


string Robot::get_Aria_port() {
    return *Aria_port;
}

void Robot::set_Aria_port(string value) {
    do
    {
    } while (!Aria_port_escribible);
    Aria_port_escribible=false;
    *Aria_port = value;
    Aria_port_escribible=true;
}

string Robot::get_Laser_port() {
    return *Laser_port;
}

void Robot::set_Laser_port(string value) {
    do
    {
    } while (!Laser_port_escribible);
    Laser_port_escribible=false;
    *Laser_port = value;
    Laser_port_escribible=true;
}

string Robot::get_PTZ_port() {
    return *PTZ_port;
}

void Robot::set_PTZ_port(string value) {
    do
    {
    } while (!PTZ_port_escribible);
    PTZ_port_escribible=false;
    *PTZ_port = value;
    PTZ_port_escribible=true;
}
void Robot::setEstimatedPosition(Location newPosition) {
    do
    {
    } while (!estimatedPosition_escribible);
    estimatedPosition_escribible=false;
	estimatedPosition = newPosition;
    estimatedPosition_escribible=true;
}

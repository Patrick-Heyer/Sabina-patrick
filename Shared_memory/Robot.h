#ifndef _ROBOT_H
#define _ROBOT_H


#include "Location.h"
#include <string>
using namespace std;
#include <list>
using namespace std;
#include "Laser.h"
#include "PTZ.h"
#include "Map.h"
#include "Arm.h"
#include "Voice.h"
#include "Synthesis.h"
#include "Destination.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

class Objective;

///*! \brief  Singleton instance of the robots shared memory
// *
// *  This object is a singleton that contains all the relevant information of
// *  the robot. This information is just a reference to share information it
// *  has no real acces to the hardware, and it is only used by the modules
// *  to share information. <b>So the information contained here could
// *  be wrong if a module dosnt update corectly.</b> Information about 
// * the way singletons work is available in designe paterns books
// */
class Robot {
  private:
    static Robot * m_singleton;

    ///*! \brief Boolean true if main system has started
    // *
    // *  Used to indicate the plugins that all subsystems have started
    // */
public:

  bool Main_system;
  
  bool localized;
  
  bool seguir;
  
  int test_time;
  int tiempo_salida;
  
  int face_frame;

    ///*! \brief Position of the robot
    // *
    // *  Location of the robot on the map x, y, angle,
    // */
    Location Position;
	
	///*! \brief Position of the robot
	
    // *
    // *  Location of the robot estimated by the localization module (in meters).
    // */
    Location estimatedPosition;

    ///*! \brief Lineal velocity of the robot
    // *
    // *  Contains the velocity for lineal motion aka. forwards/backwards in meters/second
    // */
    float Lineal_velocity;

    ///*! \brief Angular velocity of the robot
    // *
    // *  Contains the velocity for rotation aka. left/right in radians/second
    // */
    float Angular_velocity;
   

    ///*! \brief String containing the action selected by the MDP
    // *
    // *  String with the name of the optimal action selected by the MDP.
    // */
    string * Action;

    ///*! \brief List of point that the robot will travel
    // *
    // *  List of Locations (x, y, angle) that define the path the robot should travel. After the robot gets
    // *  to a Location it should be removed from the list.
    // */
    list<Location> *Route;

    ///*! \brief List of known places on the map
    // *
    // *  List of places on the map Name indicates the name of the place on the map 
    // *  (the name has to be unique)
    // */
    map<string, Destination> *Destinations;

    ///*! \brief List of known Objectives on the map
    // *
    // *  List of Objectives on the map Name indicates the name the objective 
    // *  Type indicates if it is an object or a person
    // *  Visual_ID is the reference of the vision system
    // *  Last_x, Last_y indicate the last coordinates the object was seen (it could have moved)
    // *  (the name has to be unique)
    // */
    map<string, Objective> *Objectives;

    ///*! \brief Contains the name of the last seen object
    // *
    // *  Contains the name of the last seen object.
    // */
    // 
    string * Last_objective;

    ///*! \brief Contains the name of the destination where the robot should travel
    // *
    // *  Contains the name of the destination where the robot should travel.
    // */
    
    string * Current_destination;
    string * prev_destination;

    ///*! \brief Port that aria will use
    // *
    // *  Path of the port that will be used by Aria to comunicate with the robots hardware
    // */
    std::string *Aria_port;
    
    ///*! \brief Port that the laser will use
    // *
    // *  Path of the port that will be used to conect to the laser
    // */
    std::string *Laser_port;
    
    ///*! \brief Port for PTZ control
    // *
    // *  PTZ control port for panning tilting and zooming
    // */
    std::string *PTZ_port;
    


  public:
	  
	Laser *laser;
    ///*! \brief Poiter to the PTZ information
    // *
    // *  
    // */
    PTZ Ptz;

    ///*! \brief Contains a poiter to the map the robot is using
    // *
    // *  
    // */
    Map Enviroment;

    ///*! \brief Pointer to the Arm object
    // *
    // *  
    // */
    Arm Arm_device;

    ///*! \brief Pointer to the voice recognition object
    // *
    // *  
    // */
    Voice Microphone;

    ///*! \brief Pointer to the Synthesis object
    // *
    // *  
    // */
    Synthesis Sintetizer;    

  protected:
    ///*! \brief Private constructor 
    // *
    // *  The constructor has to be private or protected so the singleton patern works
    // */
    Robot();


  public:
    ///*! \brief Starts the singleton
    // *
    // *  Starts the singleton similar to a constructor. Its the only way to set initial values
    // *  since the constructor is private. The initial values are read from the a configuration
    // *  file given by filename
    // */
    void Initialize(string filename);

    ///*! \brief Returns a pointer to the Robot object
    // *
    // *  Returns a pointer to the only copy in memory of the Robot object. If no object exist it
    // *  will initialize the instance befor returning the pointer \return Robot
    // */
    Robot & getInstance();

    ///*! \brief Gets the position of the robot
    // *
    // *  Gets the position of the robot (x, y, angle) \return Position
    // */
    Location get_Position();

    ///*! \brief Sets the position of the robot
    // *
    // *  Sets the position of the robot (x, y, angle)
    // */
    void setPosition(Location value);

    ///*! \brief Gets the Lineal velocity of the robot
    // *
    // *  Gets the Lineal velocity of the robot \return Lineal_velocity
    // */
    float get_Lineal_velocity();

    ///*! \brief Sets the Lineal velocity of the robot
    // *
    // *  Sets the Lineal velocity of the robot 
    // */
    void set_Lineal_velocity(float value);

    ///*! \brief Gets the Angular velocity of the robot
    // *
    // *  Gets the Angular velocity of the robot \return Angular_velocity
    // */
    float get_Angular_velocity();

    ///*! \brief Sets the Angular velocity of the robot
    // *
    // *  Sets the Angular velocity of the robot 
    // */
    void set_Angular_velocity(float value);

    ///*! \brief Gets the name of the destination
    // *
    // *  Gets the name of the destination of the robot. There is no safty check to see if the
    // *  destination exists \return Current_destination
    // */
    string get_Current_destination();
    
    ///*! \brief Sets the name of the destination
    // *
    // *  Sets the name of the destination of the robot. There is no safty check to see if the
    // *  destination exists
    // */
    void set_Current_destination(string value);

    
    void set_prev_destination(string value);
    ///*! \brief Gets the name of last seen of the last seen object
    // *
    // *  Gets the name of last seen of the last seen object. There is no safty check to 
    // *  see if the name of the object exists \return Last_objective
    // */
    string get_Last_objective();

    ///*! \brief Sets the name of last seen of the last seen object
    // *
    // *  Sets the name of last seen of the last seen object. There is no safty check to 
    // *  see if the name of the object exists.
    // */
    void set_Last_objective(string value);

    ///*! \brief Gets the route of the robot
    // *
    // *  Gets a list of Locations that form the route or path  \return Route
    // */
    list<Location> & get_Route();

    ///*! \brief Sets the route of the robot
    // *
    // *  Sets a list of Locations that form the route or path.
    // */
    void set_Route(list<Location>  value);

    ///*! \brief Gets the name of the action decided by the MDP
    // *
    // *   Gets the name of the action decided by the MDP \return Action
    // */
    string get_Action();

    ///*! \brief Sets the name of the action decided by the MDP
    // *
    // *   Sets the name of the action decided by the MDP 
    // */
    void set_Action(string value);

    ///*! \brief Adds a location to the Route
    // *
    // *  Adds a location to the end of the list of locations containd in Route
    // */
    void Add_Location_to_path(const Location & location);

    ///*! \brief Loads the list of known locations from file
    // *
    // *  Loads the list of known locations from file \return false if file not found
    // */
    bool Load_Destinations(string filename);

    ///*! \brief Deletes a location to the Route
    // *
    // *  Deletes the first element of the Route list
    // */
    void Delete_Location_from_path();
	
	    ///*! \brief Clears the Route
    // *
    // *  Clears all elements in the Route list
    // */
    void Clear_path();
    

    string get_Aria_port();
    void set_Aria_port(string value);
    
    string get_Laser_port();
    void set_Laser_port(string value);
    
    string get_PTZ_port();
    void set_PTZ_port(string value);
	
	///*! \brief Sets the estimated position of the robot
    // *
    // *  Sets the Angular velocity of the robot 
    // */
    void setEstimatedPosition(Location newPosition);

};
#endif

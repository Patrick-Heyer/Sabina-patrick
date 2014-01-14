#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <ArRobot.h>
#include <Aria.h>
/**
 * @brief Encapsulates the robot object. Here are implemented the navigation of all the points given by the navigation planning module.
 *
 *In this class is initialized the comunication with the robot and laser sensor (hardware). The base library used is  Aria by mobileRobots. This class use a ArActionGoto object to control robot, and use an ArActionAvoidFront object to do a simple obstacle avoidance.The path list contains all the points (x, y positions) that the robot has to visit. This class sets the difference distances to reach a point in the path.
 *
 *
 * @author Miguel Palacios  mkpalacio@gmail.com
 * @version 1.0
 * @date 30/06/2012
 * @see Aria, ArRobot, ArActionGoto, ArActionAvoidFront (in the Aria documentation http://www.ai.rug.nl/vakinformatie/pas/content/Aria-manual/main.html).
 *
 **/


class Controller
{
public:
    /**
    * Creates a Controller object.
    * @param robotPort the port for the robot connection
    *@param laserPort the port for the laser connection
    **/
    Controller(char *robotPort = "/dev/ttyUSB0",char *laserPort = "/dev/ttyUSB1");
    ///Destructor
    virtual ~Controller();
    /**
    * This method sets the path that the robot have to follows.
    * @param path the path to follows.
    * */
    void setPath(std::list<ArPose> path);
    /**
    * Sets the robot odometry.
    * @param x the coorddenate x
    * @param y the coordenate y
    * @param heading the heading
    **/
    void setOdometer (double x = 0, double y = 0, double heading = 0);
    /**
    * This method returns true if there is an obstacle in the distance specified. The obstacle is detected using the laser sensor.
    * @param distance minimal distance where to find an obstacle
    **/
    bool isThereObstacle(double distance);
    /**
    * This method executes the movements to follow the path.
    * */
    void moveRobot();


    void follow();

    void repeat();

    /**
    * This method returns the X coordenate of the robot
    * @return the X coordenate of the robot position.
    **/
    double getRobotPositionX();
    /**
    * This method returns the Y coordenate of the robot
    * @return the Y coordenate of the robot position.
    **/
    double getRobotPositionY();
    /**
    * This method returns the robot heading.
    * @return the robot heading.
    **/
    double getRobotHeading();
    /**
    * This method sets the Absolute Maximal Translational Velocity for the robot.
    * @param velocity the max velocity.
    * @warning exist a tradeoff between absolute translational veolicity and absolute maximal rotation acceleration.
    * */
    void setAbsoluteMaxTransVelocity(double velocity);
    /**
    * This method sets the Absolute Maximal Rotational Acceleration for the robot.
    * @param acceleration the max acceleration
    * @warning exist a tradeoff between absolute translational veolicity and absolute maximal rotations acceleration.
    * */
    void setAbsoluteMaxRotAcceleration(double acceleration);
    /**
    * This method sets the minimal close distance for the robot when visits a point.
    * @param minimalCloseDistance the minimal close distance
    * * @warning minimalCloseDistance can not to be bigger to maximalCloseDistance.
    * */
    void setMinimalCloseDistance(double minimalCloseDistance = 100);
    /**
    * This method sets the maximal close distance for the robot when visits a point.
    * @param maximalCloseDistance the maximal close distance
    * @warning maximalCloseDistance has to be bigger or equals to minimalCloseDistance.
    * */
    void setMaximalCloseDistance(double maximalCloseDistance = 150);
    /**
    * This method sets the time that the robot has to reach a point. Is used to detect obstacle avoidance, if after this time the point could'n reached then go to the next point .
    *
    * @param timeToReachPoint the time (in ms) to reach the current point in the path.
    *
    * */
    void setTimeToReachPoint(double timeToReachPoint = 20000);
    /**
    * This method sets the close distance for the robot when visits a point after timeToReachPoint.
    * @param closeDistanceWhenObstacle the close distance after the timeToReachPoint has been reached.
    * */
    void setCloseDistanceWhenObstacleAvoidance(double closeDistanceWhenObstacle = 800);

    void setRobotHeading(double finalHeading = 0);
private:
    ///The robot port
    char *robotPort;
    ///The laser port
    char *laserPort;
    /// The robot @see ArRobot
    ArRobot *robot;
    /// The parser object.
    ArArgumentParser *parser;
    ///The connector for the robot.
    ArRobotConnector *robotConnector;
    ///The connector for the laser sensor.
    ArLaserConnector *laserConnector;
    ///The action used for avoiding obstacles @see ArActionAvoidFront
    ArActionAvoidFront *actionAvoidFront;
    ///The ArAction object used to move the robot to the next point on the path @see ArActionGoto
    ArActionGoto *actionGoto;
    ///The path
    std::list<ArPose> path;
    ///This method initializes the robot connection.
    void initialize();
    ///The minimal close distance when the robot is moving
    double minimalCloseDistance;
    ///The maximal close distance when the robot is moving
    double maximalCloseDistance;
    ///The time that the robot has to reach a point in the path
    double timeToReachPoint;
    ///The close distance used when time to reach a point has been exceeded.
    double closeDistanceWhenObstacle;
};

#endif // CONTROLLER_H

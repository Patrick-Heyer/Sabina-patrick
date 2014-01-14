#include "Controller.h"
#include "../Shared_memory/Robot.h"


Robot *patrolbot;

Controller::Controller(char* robotPort, char* laserPort)
{
    this->robotPort = robotPort;
    this->laserPort = laserPort;
    initialize();

}


Controller::~Controller()
{

}

void Controller::initialize()
{
    Aria::init();
    robot = new ArRobot();
    actionGoto = new ArActionGoto("goto", ArPose(0.0, 0.0, 0.0), 100, 300, 14, 7);
    actionAvoidFront = new ArActionAvoidFront ("Avoid Front Near", 150,0,2);
    int argsNumber;

    robot->addAction(actionAvoidFront,100);
    robot->addAction(actionGoto,50);
    
    setMaximalCloseDistance();
    setMinimalCloseDistance();
    setTimeToReachPoint();
    setCloseDistanceWhenObstacleAvoidance();
    
    argsNumber = 5;
    char *arguments[argsNumber];
    arguments[0]="./arg_test";
    arguments[1]="-rp";
    arguments[2]=robotPort;//ttyUSB0
    arguments[3]="-lp";
    arguments[4]=laserPort;//ttyUSB1

    parser = new ArArgumentParser (&argsNumber, arguments);
    parser->loadDefaultArguments();
    //Always try to connect to the first laser:
    parser->addDefaultArgument("-connectLaser");

    //Connect to the robot, get some initial data from it such as type and name,
    //and then load parameter files for this robot.
    robotConnector = new ArRobotConnector (parser, robot);
    if (!robotConnector->connectRobot())
    {
        ArLog::log(ArLog::Terse, "Could not connect to the robot.");
        if (parser->checkHelpAndWarnUnparsed())
        {
            Aria::logOptions();
        }
    }
    //Trigger argument parsing
    if (!Aria::parseArgs() || !parser->checkHelpAndWarnUnparsed())
    {
        Aria::logOptions();

    }
//     setOdometer(0,0,0);
    robot->runAsync(true);
    laserConnector = new ArLaserConnector (parser, robot, robotConnector);
    if (!laserConnector->connectLasers())
    {
        ArLog::log(ArLog::Normal, "Warning: unable to connect to requested lasers");
    }
}

void Controller::setOdometer(double x, double y, double heading)
{
    robot->lock();
    robot->moveTo(ArPose(x,y,heading));
	
    robot->unlock();
    ArLog::log(ArLog::Normal, "simpleConnect: Pose=(%.2f,%.2f,%.2f), Trans. Vel=%.2f, Battery=%.2fV", robot->getX(), -1*robot->getY(), robot->getTh(), robot->getVel(), robot->getBatteryVoltage());
}

bool Controller::isThereObstacle(double distance)
{
    double distanceToObstacle;
    double angle;
    distanceToObstacle = robot->checkRangeDevicesCurrentPolar(-20,20,&angle);
    if (distanceToObstacle > distance) {
        return false;
    } else {
        return true;
    }

}

void Controller::setPath(std::list<ArPose> path)
{
    this->path = path;
}

void Controller::moveRobot()
{
    int point;
    point = 1;
    ArTime start;
    Location temp;
	
    robot->enableMotors();
    for (std::list<ArPose>::iterator listIterator = path.begin(); listIterator != path.end(); listIterator++)
    {
        printf("Point: %d,  X: %f  Y:  %f\n",point++, (*listIterator).getX(), (*listIterator).getY());

        if ( listIterator != ( --path.end() ))
        {
            actionGoto->setCloseDist(maximalCloseDistance);
        }
        else
        {
            actionGoto->setCloseDist(minimalCloseDistance);
        }
        start.setToNow();
        actionGoto->setGoal(*listIterator);
        bool band= 1;
        while (!actionGoto->haveAchievedGoal())
        {
            if (start.mSecSince() > timeToReachPoint && band)
            {
                actionGoto->setCloseDist(closeDistanceWhenObstacle);
                band = 0;
                printf( "++CloseDist = %f \n", actionGoto->getCloseDist());
            }
            temp.set(robot->getPose().getX()/10, (robot->getPose().getY()/10)*-1, robot->getTh());
			patrolbot->getInstance().setPosition(temp);
            
        }
    }
    robot->disableMotors();

}

double Controller::getRobotPositionX()
{
    return robot->getPose().getX();
}

double Controller::getRobotPositionY()
{
    return robot->getPose().getY();
}

double Controller::getRobotHeading()
{
    return robot->getPose().getTh();
}

void Controller::setAbsoluteMaxRotAcceleration(double acceleration)
{
  robot->setAbsoluteMaxRotAccel(acceleration);
}

void Controller::setAbsoluteMaxTransVelocity(double velocity)
{
  robot->setAbsoluteMaxTransVel(velocity);
}
void Controller::setMaximalCloseDistance(double maximalCloseDistance)
{
  this->maximalCloseDistance = maximalCloseDistance;
}
void Controller::setMinimalCloseDistance(double minimalCloseDistance)
{
 this->minimalCloseDistance = minimalCloseDistance;
}
void Controller::setTimeToReachPoint(double timeToReachPoint)
{
  this->timeToReachPoint = timeToReachPoint; 
}
void Controller::setCloseDistanceWhenObstacleAvoidance(double closeDistanceWhenObstacle)
{
  this->closeDistanceWhenObstacle = closeDistanceWhenObstacle;
}





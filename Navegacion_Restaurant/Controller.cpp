#include "Controller.h"
#include "../Shared_memory/Robot.h"
#include "../Human/Human.h"
#include "../Human/joint.h"

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
    //and then load parameter files for this robot->
    robotConnector = new ArRobotConnector (parser, robot);
    if (!robotConnector->connectRobot())
    {
        ArLog::log(ArLog::Terse, "Could not connect to the robot->");
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
    
    
//     robot->setTransAccel(250);
// 	robot->setTransDecel(300);
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
	Location temp;
void Controller::moveRobot()
{
    int point;
    point = 1;
    ArTime start;
    Location temp;
	
    robot->enableMotors();
	int aux =0;
	//we delete the first element whom is the current position
	if (path.size()>1)
	path.pop_front();
	
	
    for (std::list<ArPose>::iterator listIterator = path.begin(); listIterator != path.end(); listIterator++)
    {
        if ( listIterator != ( --path.end() ))
        {
            actionGoto->setCloseDist(maximalCloseDistance);
			
			if (listIterator == path.begin()){
			double angle = robot->getPose().findAngleTo(*listIterator);
			setRobotHeading(angle);
			//ArUtil::sleep(200);
			robot->enableMotors();
			}
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
	  temp.set(getRobotPositionX()/50, (getRobotPositionY()/50)*-1, getRobotHeading());
 		patrolbot->getInstance().setPosition(temp);
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

void Controller::follow()
{    std::map<std::string, skeletonJoint>::iterator iter;
    double angle;
    int speed=0, rot=0;
    bool obstaculolaser=false;
    skeletonJoint R_arm, L_arm, Torso;
	robot->enableMotors();
	robot->disableMotors();
	robot->enableMotors();
	int veces_r=0;
	int veces_l=0;
	for(;;)
	{
	
        if (Human::getInstance().tracking==false)
        {
            robot->setVel(0);
            robot->setDeltaHeading(0);
            continue;   
        }
        
        for(iter=Human::getInstance().Skeleton->begin(); iter !=Human::getInstance().Skeleton->end(); ++iter)
        {
            if(iter->second.Get_ID()==3) Torso=iter->second;
            if(iter->second.Get_ID()==9) L_arm=iter->second;
            if(iter->second.Get_ID()==15) R_arm=iter->second;
        }
        
        if (R_arm.Get_y()>Torso.Get_y() || L_arm.Get_y()>Torso.Get_y())
        {
            speed=0;
            if(R_arm.Get_y()>Torso.Get_y())
            {
				veces_r++;
				if(veces_r>=10)
				{
                patrolbot->getInstance().Sintetizer.set_Phrase("i learnd this place as DININGROOM");
				sleep(2);
				veces_r=-1000;
				}
            }
            else
            {
				veces_l++;
				if(veces_l>=10)
				{
                patrolbot->getInstance().Sintetizer.set_Phrase("i learnd this place as KITCHEN");
				sleep(2);
				veces_l=-1000;
				}
            }
        }
        
        
        
        else 
        {

			if(veces_l>=0)
			{
				veces_l=0;
			}
			if(veces_r>=0)
			{
				veces_r=0;
			}
                if (Torso.Get_z()<600  )
                {
                    speed=-200;
                }
                else 
                {
                    if (Torso.Get_z()>1200&& Torso.Get_z()<2500 )
                    {
                        // robot->move(10*15);
                        speed=Torso.Get_z()-1200;
                        
                    }
                }
                
                if (Torso.Get_x() > 70 )
                {
                    rot=60;
                    
                }
                else if (Torso.Get_x() < -70 )
                {
                    rot=-60;
                    
                }
                else
                {
                    rot=0;
                } 
            
        }
        
        
        robot->setVel(speed);
        robot->setDeltaHeading(rot);
        
    }
	
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
void Controller::setRobotHeading ( double finalHeading )
{
	robot->enableMotors();
	robot->setDirectMotionPrecedenceTime(1000);
	robot->setHeading(finalHeading);
	
	while (!robot->isHeadingDone()){};
	
	robot->clearDirectMotion();
	robot->disableMotors();
cout<<"PRecedence TIME"<<robot->getDirectMotionPrecedenceTime();

	
}





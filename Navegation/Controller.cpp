#include "Controller.h"
#include "../Shared_memory/Robot.h"
#include "../Shared_memory/Laser.h"

#include "../Human/Human.h"
#include "../Human/joint.h"

//this class is related with main2 class
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
    initialXPosition = 0;
    initialYPosition = 0;

    robot->addAction(actionAvoidFront,100);
    robot->addAction(actionGoto,50);

    setMaximalCloseDistance();
    setMinimalCloseDistance();
    setTimeToReachPoint();
    setCloseDistanceWhenObstacleAvoidance();

    argsNumber = 7;
    char *arguments[argsNumber];
    arguments[0]="./arg_test";
    arguments[1]="-rp";
    arguments[2]=robotPort;//ttyUSB0
    arguments[3]="-lp";
    arguments[4]=laserPort;//ttyUSB1
    arguments[5]="-laserincrement";
    arguments[6]="half";



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

    robot->runAsync(true);
    laserConnector = new ArLaserConnector (parser, robot, robotConnector);
    if (!laserConnector->connectLasers())
    {
        ArLog::log(ArLog::Normal, "Warning: unable to connect to requested lasers");

    }

    //robot->setTransDecel(200);
    //robot->setTransAccel(300);

}

void Controller::setOdometer(double x, double y, double heading)
{
    robot->lock();
    robot->moveTo(ArPose(x,y,heading));
    robot->unlock();
    ArLog::log(ArLog::Normal, "Robot odometer setting to: Pose=(%.2f,%.2f,%.2f), Trans. Vel=%.2f, Battery=%.2fV \n", robot->getX(), robot->getY(), robot->getTh(), robot->getVel(), robot->getBatteryVoltage());
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
    int aux =0;
    //we delete the first element whom is the current position
    if (path.size()>1)
        path.pop_front();

    for (std::list<ArPose>::iterator listIterator = path.begin(); listIterator != path.end(); listIterator++)
    {

        getLaserScanFromRobot();
        if ( listIterator != ( --path.end() ))
        {
            actionGoto->setCloseDist(maximalCloseDistance);

// 			if (listIterator == path.begin()){
// 			double angle = robot->getPose().findAngleTo(*listIterator);
// 			setRobotHeading(angle);
// 			//ArUtil::sleep(200);
// 			robot->enableMotors();
// 			}
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
            getLaserScanFromRobot();
            if (start.mSecSince() > timeToReachPoint && band)
            {
                actionGoto->setCloseDist(closeDistanceWhenObstacle);
                band = 0;
                printf( "++CloseDist = %f \n", actionGoto->getCloseDist());
            }
            getLaserScanFromRobot();
            temp.set(robot->getPose().getX()/50, (robot->getPose().getY()/50)*-1, robot->getTh());
            patrolbot->getInstance().setPosition(temp);
        }

        if (patrolbot->getInstance().localized == true)
        {
            //cout <<"Initial X position: " << initialXPosition << "Initial Y position: " << initialYPosition<<endl;
            printf("Point reached: %d,  X: %f  Y:  %f\n",point++, (*listIterator).getX(), (*listIterator).getY());
            cout << "EX:"<<patrolbot->getInstance().estimatedPosition.get_X() <<" EY:"<<patrolbot->getInstance().estimatedPosition.get_Y()<< endl;
            double newXPosition=patrolbot->getInstance().estimatedPosition.get_X()+initialXPosition;
            double newYPosition=patrolbot->getInstance().estimatedPosition.get_Y()+initialYPosition;
            cout << "Last Odometry X:"<<getRobotPositionX()<<" Y:"<<getRobotPositionY()<<" Z:" <<getRobotHeading()<<endl;
            setOdometer(newXPosition, newYPosition, getRobotHeading());

            temp.set(robot->getPose().getX()/50, (robot->getPose().getY()/50)*-1, robot->getTh());
            patrolbot->getInstance().setPosition(temp);

            //cout << "LOCALIZADOII X:"<<getRobotPositionX()<<" Y:"<<getRobotPositionY()<<" :" <<getRobotHeading()<<endl;

        }



    }


}


void Controller::follow()
{
    std::map<std::string, skeletonJoint>::iterator iter;
    int speed = 0, rot = 0;
    skeletonJoint R_Hand, L_Hand, Torso;
    Location temp;
    robot->enableMotors();
    robot->disableSonar();
    this->setAbsoluteMaxRotAcceleration(40);
    this->setAbsoluteMaxTransVelocity(160);


    Destination dest;
    temp.set(robot->getPose().getX()/50, (robot->getPose().getY()/50)*-1, robot->getTh());

    dest.set_Coordinate(temp);

    dest.set_Name("START");

    patrolbot->getInstance().Destinations->insert(std::make_pair( "START",dest));


    for(;;)
    {
        for ( iter=Human::getInstance().Skeleton->begin(); iter !=Human::getInstance().Skeleton->end(); ++iter )
        {
            if ( iter->second.Get_name()=="TORSO" ) Torso=iter->second;
            if ( iter->second.Get_name()=="LEFT_HAND" ) L_Hand=iter->second;
            if ( iter->second.Get_name()=="RIGHT_HAND" ) R_Hand=iter->second;
        }


        if ( R_Hand.Get_y() >Torso.Get_y() && L_Hand.Get_y() >Torso.Get_y() )
        {
            std::cout << "vi tus manos" << std::endl;

            return;
        }

        else if ( R_Hand.Get_y() >Torso.Get_y() || L_Hand.Get_y() >Torso.Get_y() )
        {

            if ( R_Hand.Get_y() >Torso.Get_y() )
            {

                dest.set_Coordinate(temp);
                dest.set_Name("CHECK1");
                patrolbot->getInstance().Destinations->insert(std::make_pair( "CHECK1",dest));
                patrolbot->getInstance().Sintetizer.set_Phrase("i learnd this place as check point ONE");
                sleep(3);
            }
            if ( L_Hand.Get_y() >Torso.Get_y() )
            {
                dest.set_Coordinate(temp);
                dest.set_Name("CHECK2");
                patrolbot->getInstance().Destinations->insert(std::make_pair( "CHECK2",dest));
                patrolbot->getInstance().Sintetizer.set_Phrase("i learnd this place as check point TWO");
                sleep(3);
            }
        }
        else
        {
            if ( Torso.Get_z() <1000 )
            {
                speed=-200;
            }
            if ( Torso.Get_z() >1300&& Torso.Get_z() <2500 )
            {
                speed=Torso.Get_z()-1300;

            }
            if ( Torso.Get_z() >2500 )
            {
                patrolbot->getInstance().Sintetizer.set_Phrase("please slow down");
                speed=200;
            }

            rot=Torso.Get_x() /70;
        }
        if(isThereObstacle ( 400 ))
        {
            robot->setVel ( 0 );
            robot->setDeltaHeading ( 0 );
        }
        else
        {
            robot->setVel ( speed );
            robot->setDeltaHeading ( rot );

        }

        temp.set(robot->getPose().getX()/50, (robot->getPose().getY()/50)*-1, robot->getTh());
        patrolbot->getInstance().setPosition(temp);


    }
}



void Controller::setInitialRobotPositionFromImage(int iniXPosition, int iniYPosition)
{
    initialXPosition = iniXPosition;
    initialYPosition = iniYPosition;
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

void Controller::getLaserScanFromRobot()
{
    ArLaser *laser = robot->findLaser(1);
    laser->lockDevice();
    //if (laser->isConnected()){
    vector <ArSensorReading> *readings = laser->getRawReadingsAsVector();
    //vector <ArSensorReading> *readings = laser->getAdjustedRawReadingsAsVector();

    laser->unlockDevice();
// 		laser->getLastReadingTime();
// 		laser->getReadingCount();
// 		laser->resetLastCumulativeCleanTime();
// 		laser->clearCumulativeReadings();
// 		laser->getMinDistBetweenCumulative();
// 		laser->setMaxDistToKeepCumulative();
// 		laser->setMinDistBetweenCurrent();

    std::vector<ArSensorReading>::const_iterator it;
    int cont=1;
    std::vector<float> rawScan;
    if (readings->size()>0) {
        rawScan.clear();
        //cout<<laser->getMaxSecondsToKeepCurrent();
        for (it = readings->begin(); it != readings->end(); ++it)
        {
            //printf("%d,%f ",cont++,(*it).getRange()/1000.0);
            rawScan.push_back((*it).getRange()/1000.0);
        }
        cont=1;
        patrolbot->getInstance().laser->setLaserScan(rawScan);
        //printf("*** \n ");
// 		for (it = readings->begin(); it != readings->end(); ++it)
// 		{
// 			printf("%d,%d ",cont++,(*it).getPose().findDistanceTo(robot->getPose()));
//
// 		}
        //printf("Despues del for \n");
        //}
// 		laser->unlockDevice();
    }
}



void Controller::setTransAccel(double transAccel)
{
    robot->setTransAccel(transAccel);
    cout<<"Rotational Acceleration"<<robot->getRotAccel() << std::endl;
    cout<<"Rotational deceleration"<<robot->getRotDecel() << std::endl;
}
void Controller::setTransDecel(double transDecel)
{
    robot->setTransDecel(transDecel);
}

void Controller::setRobotHeading ( double finalHeading )
{
    robot->enableMotors();
    robot->setDirectMotionPrecedenceTime(1000);
    robot->setHeading(finalHeading);

    while (!robot->isHeadingDone()) {
        getLaserScanFromRobot();
    };

    robot->clearDirectMotion();
    robot->disableMotors();
    cout<<"PRecedence TIME"<<robot->getDirectMotionPrecedenceTime();


}

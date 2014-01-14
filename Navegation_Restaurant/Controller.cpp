#include "Controller.h"
#include "../Shared_memory/Robot.h"
#include "../Human/Human.h"
#include "../Human/joint.h"

Robot *patrolbot;

Controller::Controller ( char *robotPort, char *laserPort )
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
    actionGoto = new ArActionGoto ( "goto", ArPose ( 0.0, 0.0, 0.0 ), 100, 300, 14, 7 );
    actionAvoidFront = new ArActionAvoidFront ( "Avoid Front Near", 150, 0, 2 );
    int argsNumber;

    robot->addAction ( actionAvoidFront, 100 );
    robot->addAction ( actionGoto, 50 );

    setMaximalCloseDistance();
    setMinimalCloseDistance();
    setTimeToReachPoint();
    setCloseDistanceWhenObstacleAvoidance();

    argsNumber = 5;
    char *arguments[argsNumber];
    arguments[0] = "./arg_test";
    arguments[1] = "-rp";
    arguments[2] = robotPort; //ttyUSB0
    arguments[3] = "-lp";
    arguments[4] = laserPort; //ttyUSB1

    parser = new ArArgumentParser ( &argsNumber, arguments );
    parser->loadDefaultArguments();
    //Always try to connect to the first laser:
    parser->addDefaultArgument ( "-connectLaser" );

    //Connect to the robot, get some initial data from it such as type and name,
    //and then load parameter files for this robot->
    robotConnector = new ArRobotConnector ( parser, robot );

    if ( !robotConnector->connectRobot() )
    {
        ArLog::log ( ArLog::Terse, "Could not connect to the robot->" );

        if ( parser->checkHelpAndWarnUnparsed() )
        {
            Aria::logOptions();
        }
    }

    //Trigger argument parsing
    if ( !Aria::parseArgs() || !parser->checkHelpAndWarnUnparsed() )
    {
        Aria::logOptions();

    }

    //     setOdometer(0,0,0);
    robot->runAsync ( true );
    laserConnector = new ArLaserConnector ( parser, robot, robotConnector );

    if ( !laserConnector->connectLasers() )
    {
        ArLog::log ( ArLog::Normal, "Warning: unable to connect to requested lasers" );
    }


    //     robot->setTransAccel(250);
    //  robot->setTransDecel(300);
}

void Controller::setOdometer ( double x, double y, double heading )
{
    robot->lock();
    robot->moveTo ( ArPose ( x, y, heading ) );

    robot->unlock();
    ArLog::log ( ArLog::Normal, "simpleConnect: Pose=(%.2f,%.2f,%.2f), Trans. Vel=%.2f, Battery=%.2fV", robot->getX(), -1 * robot->getY(), robot->getTh(), robot->getVel(), robot->getBatteryVoltage() );
}

bool Controller::isThereObstacle ( double distance )
{
    double distanceToObstacle;
    double angle;
    distanceToObstacle = robot->checkRangeDevicesCurrentPolar ( -20, 20, &angle );

    if ( distanceToObstacle > distance )
    {
        return false;
    }

    else
    {
        return true;
    }

}

void Controller::setPath ( std::list<ArPose> path )
{
    this->path = path;
}

void Controller::moveRobot()
{
    ArTime start;
    Location temp;

    robot->enableMotors();
    int aux = 0;

    //we delete the first element whom is the current position
    if ( path.size() > 1 )
        path.pop_front();

    for ( std::list<ArPose>::iterator listIterator = path.begin(); listIterator != path.end(); listIterator++ )
    {
        if ( listIterator != ( --path.end() ) )
        {
            actionGoto->setCloseDist ( maximalCloseDistance );

            if ( listIterator == path.begin() )
            {
                double angle = robot->getPose().findAngleTo ( *listIterator );
                setRobotHeading ( angle );
                //ArUtil::sleep(200);
                robot->enableMotors();
            }
        }

        else
        {
            actionGoto->setCloseDist ( minimalCloseDistance );
        }

        start.setToNow();
        actionGoto->setGoal ( *listIterator );
        bool band = 1;

        while ( !actionGoto->haveAchievedGoal() )
        {
            if ( start.mSecSince() > timeToReachPoint && band )
            {
                actionGoto->setCloseDist ( closeDistanceWhenObstacle );
                band = 0;
                printf ( "++CloseDist = %f \n", actionGoto->getCloseDist() );
            }


            temp.set(robot->getPose().getX()/50, (robot->getPose().getY()/50)*-1, robot->getTh());
            patrolbot->getInstance().setPosition(temp);
        }

    }

    robot->disableMotors();

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

void Controller::repeat()
{
    std::map<std::string, skeletonJoint>::iterator iter;
    skeletonJoint Torso;
    for ( iter = Human::getInstance().Skeleton->begin(); iter != Human::getInstance().Skeleton->end(); ++iter )
    {
        if ( iter->second.Get_name() == "TORSO" )
            Torso = iter->second;
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

void Controller::setAbsoluteMaxRotAcceleration ( double acceleration )
{
    robot->setAbsoluteMaxRotAccel ( acceleration );
}

void Controller::setAbsoluteMaxTransVelocity ( double velocity )
{
    robot->setAbsoluteMaxTransVel ( velocity );
}

void Controller::setMaximalCloseDistance ( double maximalCloseDistance )
{
    this->maximalCloseDistance = maximalCloseDistance;
}

void Controller::setMinimalCloseDistance ( double minimalCloseDistance )
{
    this->minimalCloseDistance = minimalCloseDistance;
}

void Controller::setTimeToReachPoint ( double timeToReachPoint )
{
    this->timeToReachPoint = timeToReachPoint;
}

void Controller::setCloseDistanceWhenObstacleAvoidance ( double closeDistanceWhenObstacle )
{
    this->closeDistanceWhenObstacle = closeDistanceWhenObstacle;
}

void Controller::setRobotHeading ( double finalHeading )
{
    robot->enableMotors();
    robot->setDirectMotionPrecedenceTime ( 1000 );
    robot->setHeading ( finalHeading );

    while ( !robot->isHeadingDone() )
        {};

    robot->clearDirectMotion();

    robot->disableMotors();

}





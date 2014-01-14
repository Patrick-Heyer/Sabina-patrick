/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   plugin1.h
/// \author Martin Reddy
/// \brief  An example plugin built against the plugin API.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#define WIDTH 1440
#define HEIGHT 900

#include <iostream>
#include <pthread.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "../Gui/gui.h"
#include "../Gui/console.h"
#include "../Gui/video.h"
#include "../Plugin_API/pluginapi.h"
#include "../Shared_memory/Robot.h"

#include "../Gui/input_singleton.h"
#include "../Gui/gui.h"
#include "../Gui/tab.h"
#include "../Gui/console.h"
#include "../Gui/video.h"
#include "../Gui/kinect.h"


#include "Aria.h"
#include "ArActionGoToSabina.h";

#include <ArRobot.h>

#include "../MDP/MDP.h"


Robot *patrol;
tab *current_tab;

video *vid;


#define PLUGIN_NAME "Navegacion"

class Navegacion : public IPlugin
{
public:
    void Main();
    void run();
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Navegacion;
}

PLUGIN_FUNC void DestroyRenderer(IPlugin *r)
{
    delete r;
}

PLUGIN_DISPLAY_NAME(PLUGIN_NAME);

PLUGIN_INIT()
{
    // register our new renderer
    RegisterPlugin(PLUGIN_NAME, CreatePlugin, DestroyRenderer);
    return 0;
}

void Navegacion::Main()
{

    GUI::GetInstance();
    current_tab = new tab("Navegacion");
    console *error_terminal;
    error_terminal = new console(WIDTH-600,HEIGHT*.02,600,HEIGHT/2,"error", current_tab);
    vid = new video(0,HEIGHT*.02,WIDTH/2,HEIGHT/2,"Navegacion", current_tab);

    IplImage *map;
    map=cvLoadImage(patrol->get_Instance().Enviroment.get_Image_path().c_str());
    //ArActionGotoStraight gs("goto", 300);

    ArActionGoto gt("goto", ArPose(0.0, 0.0, 0.0), 100, 300, 14, 7);
    ArActionAvoidFront af("Avoid Front Near", 150,0,2);//,0,4);

    Aria::init();
    ArRobot robot;


    int num_arg;
    num_arg=5;
    char *arg[5];
    double accel_mio = 25.0;

    arg[0]="./arg_test";
    arg[1]="-rp";
    arg[2]=(char*)patrol->get_Instance().get_Aria_port().c_str();
    arg[3]="-lp";
    arg[4]=(char*)patrol->get_Instance().get_Laser_port().c_str();

    ArArgumentParser parser(&num_arg, arg);
    parser.loadDefaultArguments();
    // Always try to connect to the first laser:
    parser.addDefaultArgument("-connectLaser");
    robot.setAbsoluteMaxRotAccel(accel_mio);

    // Connect to the robot, get some initial data from it such as type and name,
    // and then load parameter files for this robot.
    ArRobotConnector robotConnector(&parser, &robot);
    if (!robotConnector.connectRobot())
    {
        ArLog::log(ArLog::Terse, "Could not connect to the robot.");
        if (parser.checkHelpAndWarnUnparsed())
        {
            // -help not given, just exit.
            Aria::logOptions();
            //Aria::exit(1);
        }
    }
    // Trigger argument parsing
    if (!Aria::parseArgs() || !parser.checkHelpAndWarnUnparsed())
    {
        Aria::logOptions();
        //Aria::exit(1);
    }


    // Start the robot processing cycle running in the background.
    // True parameter means that if the connection is lost, then the
    // run loop ends.
    robot.runAsync(true);

    ArLog::log(ArLog::Normal, "simpleConnect: Connected.");


    ArLaserConnector laserConnector(&parser, &robot, &robotConnector);
    // try to connect to laser. if fail, warn but continue, using sonar only
    if (!laserConnector.connectLasers())
    {
        ArLog::log(ArLog::Normal, "Warning: unable to connect to requested lasers, will wander using robot sonar only.");
    }

    // Print out some data from the SIP.  We must "lock" the ArRobot object
    // before calling its methods, and "unlock" when done, to prevent conflicts
    // with the background thread started by the call to robot.runAsync() above.
    // See the section on threading in the manual for more about this.
    robot.lock();
    robot.resetTripOdometer();
    ArLog::log(ArLog::Normal, "simpleConnect: Pose=(%.2f,%.2f,%.2f), Trans. Vel=%.2f, Battery=%.2fV",
               robot.getX(), robot.getY(), robot.getTh(), robot.getVel(), robot.getBatteryVoltage());
    robot.unlock();
    robot.addAction(&af,100);
    robot.addAction(&gt,50);
    ArPose start_position(patrol->get_Instance().Position.get_Y()*10, patrol->get_Instance().Position.get_X()*10, patrol->get_Instance().Position.get_Angle() );
    robot.moveTo(start_position);


    gt.setCloseDist(150);
    //robot.setTransVelMax(patrol->get_Instance().get_Lineal_velocity());
    //robot.setRotVelMax(patrol->get_Instance().get_Angular_velocity());
    //robot.setAbsoluteMaxRotVel(90);
    robot.setAbsoluteMaxTransVel(160);
    robot.setRotVelMax(60);
    robot.enableMotors();
    std::string accion;

    Location temp;

    CvPoint center;
    vid->SetImage(map);
    for (;;)
    {
        accion=patrol->get_Instance().get_Action();
        temp.set(robot.getPose().getY()/10, robot.getPose().getX()/10, robot.getTh());
        patrol->get_Instance().set_Position(temp);

        if (accion=="entrar")
        {
            robot.enableMotors();

            printf("Iniciando: entrar \n");
            ArPose position(robot.getPose().getY(),robot.getPose().getX()+1000,0);
            gt.setGoal(position);
            gt.setSpeed(250);

            printf("inicio del while \n");

            while (!gt.haveAchievedGoal()) {
                printf("going \n");
                ArLog::log(ArLog::Normal, "simpleConnect: Pose=(%.2f,%.2f,%.2f), Trans. Vel=%.2f, Battery=%.2fV",
                           robot.getX(), robot.getY(), robot.getTh(), robot.getVel(), robot.getBatteryVoltage());
                temp.set(robot.getPose().getY()/10, robot.getPose().getX()/10, robot.getTh());
                patrol->get_Instance().set_Position(temp);
                center.x=robot.getPose().getY()/10;
                center.y=robot.getPose().getX()/10;
                cvCircle(map, center, 7, CV_RGB(0, 0, 255), -1, 8, 0);
                vid->SetImage(map);
            }
            printf("fin del while \n");

            patrol->get_Instance().set_Action(cambiar_estado("dentro_arena", "si"));
        }
        if (accion=="navegar_destino")
        {
            robot.enableMotors();
            printf("Iniciando: navegar_destino\n");
            map=cvLoadImage(patrol->get_Instance().Enviroment.get_Image_path().c_str());
            list<Location> ruta_temp;
            ruta_temp=patrol->get_Instance().get_Route();
            int punto=0;
            ArTime start;
            if (ruta_temp.size()>1)
            {

                for (std::list<Location>::iterator list_iter = ruta_temp.begin();
                        list_iter != ruta_temp.end(); list_iter++)
                {
                    center.x=(*list_iter).get_X();
                    center.y=(*list_iter).get_Y();
                    cvCircle(map, center, 2, CV_RGB(255, 0, 0), -1, 8, 0);
                }
                vid->SetImage(map);
                ArPose poseAux;

                for (std::list<Location>::iterator list_iter = ruta_temp.begin();
                        list_iter != ruta_temp.end(); list_iter++)
                {
                    print(error_terminal,"Point: %d,  X: %d  Y:  %d\n",punto, (*list_iter).get_X(), (*list_iter).get_Y());
                    printf("Point: %d,  X: %d  Y:  %d\n",punto, (*list_iter).get_X(), (*list_iter).get_Y());
                    //printf("punto %d\n",punto);
                    punto++;
                    ArPose position((*list_iter).get_Y()*10,(*list_iter).get_X()*10, (*list_iter).get_Angle());
                    print(error_terminal,"Position: %d : %f  %f %f\n ", punto, position.getX(), position.getY());

                    if ( list_iter != ( --ruta_temp.end() ))
                        gt.setCloseDist(150);
                    else
                        gt.setCloseDist(100);

                    start.setToNow();
                    gt.setGoal(position);
                    bool band= 1;
                    while (!gt.haveAchievedGoal()) {
                        if (start.mSecSince() > 20000 && band) {
                            gt.setCloseDist(800);
                            band = 0;
                            cout << "CloseDist =  "<< gt.getCloseDist()<<endl;
                        }
                        temp.set(robot.getPose().getY()/10, robot.getPose().getX()/10, robot.getTh());
                        patrol->get_Instance().set_Position(temp);
                        //printf("VLineal: %f, VAngular: %f , Acel: %f \n",robot.getVel(), robot.getRotVel(),robot.getRotAccel());
                        center.x=robot.getPose().getY()/10;
                        center.y=robot.getPose().getX()/10;
                        cvCircle(map, center, 7, CV_RGB(0, 0, 255), -1, 8, 0);
                        vid->SetImage(map);
                    }
                    poseAux = robot.getPose();
                    printf("%d : %f  %f %f\n ", punto, poseAux.getX(), poseAux.getY(), poseAux.getTh());
                }

            }

            patrol->get_Instance().set_Action(cambiar_estado("destino_alcanzado","si"));//"destinoAlcanzado","si"));
        }

        if (accion=="aproximar_personas")
        {
            patrol->get_Instance().set_Action(cambiar_estado("persona_aproximada", "si"));
        }
        else
        {
            printf("Iniciando+++: %s \n", patrol->get_Instance().get_Action().c_str());

            robot.enableMotors();
            robot.disableMotors();
            robot.requestIOPackets();
            sleep(1);
        }

    }
}

void Navegacion::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}



/**
* @file
* This file is the entery point to comunicate with the Katana 6M 450 arm
*
* The standard for Sabina plugins is used to start and run this plugin,
* its main functionality is implemented in the armkatana and armkatanaforsabina files
*
*/

#include <iostream>
#include <pthread.h>
//#include <ArRobot.h>
//#include <Aria.h>
#include "Controller.h"

#include "../Plugin_API/pluginapi.h"
#include "../Shared_memory/Robot.h"
#include "../MDP/MDP.h"
#include "../Gui/gui.h"
#include "../Gui/console.h"
#include "../Gui/video.h"


Robot *patrol;
Tab *pluginTab;
Console *logTerminal;

#define LOCAL_MAP_HEIGHT 500.0
#define LOCAL_MAP_WIDHT 500.0


#define PLUGIN_NAME "Navegation"

/**
 * @brief Navegation plugin
 *
 * This plugin is used to perform the movments of the robot based on the route planed by Navigation_Planing
 * @author     Patrick Heyer, patrickhey@prodigy.net.mx Miguel Palacios  mkpalacio@gmail.com
 * @date       jul 04, 2012
 * @version    1.0
 **/


class Navegation : public IPlugin
{
public:
    void Main();
    void stop();
    void run();
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Navegation;
}

PLUGIN_FUNC void DestroyPlugin(IPlugin *r)
{
    delete r;
}

PLUGIN_DISPLAY_NAME(PLUGIN_NAME);

PLUGIN_INIT()
{
    // register our new plugin
    RegisterPlugin(PLUGIN_NAME, CreatePlugin, DestroyPlugin);
    return 0;
}

void Navegation::Main()
{
    /**
    * \sa cambiar_estado
    **/
    std::string accion;
    Location temp;
    CvPoint center;
    std::list<ArPose> path;
    int factor=50;

    pluginTab = new Tab("Navegation");
    logTerminal = new Console(WIDTH/2,HEIGHT*.02,WIDTH,HEIGHT/2,"Log_Terminal", pluginTab);

    IplImage *tempMap;
    tempMap=cvLoadImage(patrol->getInstance().Enviroment.get_Image_path().c_str());
    patrol->getInstance().Enviroment.set_Height_px(tempMap->height);
    patrol->getInstance().Enviroment.set_Width_px(tempMap->width);

    IplImage *map= cvCreateImage(cvSize(LOCAL_MAP_HEIGHT,LOCAL_MAP_WIDHT),tempMap->depth, tempMap->nChannels);
    cvResize(tempMap,map);
    cvReleaseImage(&tempMap);

    Controller robotController ((char*)patrol->getInstance().get_Aria_port().c_str(), (char*)patrol->getInstance().get_Laser_port().c_str());

    robotController.setAbsoluteMaxTransVelocity(patrol->getInstance().get_Lineal_velocity());
    robotController.setAbsoluteMaxRotAcceleration(patrol->getInstance().get_Angular_velocity());

    robotController.setOdometer(patrol->getInstance().Position.get_X()*factor, patrol->getInstance().Position.get_Y()*-factor, patrol->getInstance().Position.get_Angle());
// 	double initialXPosition = robotController.getRobotPositionX();
// 	double initialYPosition = robotController.getRobotPositionY();
    robotController.setInitialRobotPositionFromImage(patrol->getInstance().Position.get_X()*factor, patrol->getInstance().Position.get_Y()*-factor);
    robotController.setTransAccel(250);
    robotController.setTransDecel(200);



    int dest = 1;

    for (;;)
    {
        accion=patrol->getInstance().get_Action();
        temp.set(robotController.getRobotPositionX()/factor, (robotController.getRobotPositionY()/factor)*-1, robotController.getRobotHeading());
        patrol->getInstance().setPosition(temp);
        if (accion=="entrar")
        {
            print(logTerminal,"Iniciando: entrar\n");
            printf("iniciando entrar");
            do {}
            while (robotController.isThereObstacle(1100));
            ArPose position(robotController.getRobotPositionX()+2500,robotController.getRobotPositionY(),0);
            path.clear();
            path.push_back(position);
            robotController.setPath(path);
            robotController.moveRobot();
            print(logTerminal,"Terminado: entrar\n");
            patrol->getInstance().set_Action(cambiar_estado("dentro_arena", "si"));
        }

        if(accion=="localizar")
        {
            robotController.getLaserScanFromRobot();
            ArUtil::sleep(2000);


        }

        if ( accion == "seguir" )
        {
            do
                {}
            while ( robotController.isThereObstacle ( 1100 ) );

            patrol->getInstance().Sintetizer.set_Phrase("i will follow you");
            sleep(2);
            robotController.follow();
            std::cout << "deje de seguir" << std::endl;
            patrol->getInstance().set_Action(cambiar_estado("seguimiento_fin", "si"));

        }



        if (accion=="navegar_destino")
        {


            print(logTerminal,"Iniciando: navegar_destino\n");
            list<Location> ruta_temp;
            ruta_temp=patrol->getInstance().get_Route();
            if (ruta_temp.size()>1)
            {
                path.clear();
                for (std::list<Location>::iterator list_iter = ruta_temp.begin(); list_iter != ruta_temp.end(); list_iter++)
                {
                    ArPose position((*list_iter).get_X()*factor,(*list_iter).get_Y()*-factor,(*list_iter).get_Angle());
                    path.push_back(position);

                }
                robotController.setPath(path);
                robotController.moveRobot();
            }


            if(patrol->getInstance().get_Current_destination()!="")
            {
                typedef std::map<string,Destination> StringFloatMap;
                StringFloatMap::iterator pos;
                pos = patrol->getInstance().Destinations->find(patrol->getInstance().get_Current_destination());
                cout << pos->second.get_Coordinate().get_Angle() << endl;
                robotController.setRobotHeading(pos->second.get_Coordinate().get_Angle() );
            }


            patrol->getInstance().Clear_path();
            print(logTerminal,"Terminado: navegar_destino");

            patrol->getInstance().set_Action(cambiar_estado("destino_alcanzado", "si"));
        }

        if (accion=="aproximar_persona")
        {
            print(logTerminal,"Iniciando: aproximar_persona\n");
            list<Location> ruta_temp;
            ruta_temp=patrol->getInstance().get_Route();
            if (ruta_temp.size()>0)
            {
                path.clear();
                for (std::list<Location>::iterator list_iter = ruta_temp.begin(); list_iter != ruta_temp.end(); list_iter++)
                {
                    ArPose position((*list_iter).get_X(),(*list_iter).get_Y(),(*list_iter).get_Angle());
                    path.push_back(position);

                }
                robotController.setPath(path);
                robotController.moveRobot();
                print(logTerminal,"Terminado: aproximar_persona\n");
            }
            sleep(1);
        }
    }
}

void Navegation::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}

void Navegation::stop()
{

}



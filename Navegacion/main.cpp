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


#define PLUGIN_NAME "Navegacion"

/**
 * @brief Navegacion plugin
 * 
 * This plugin is used to perform the movments of the robot based on the route planed by Navigation_Planing
 * @author     Patrick Heyer, patrickhey@prodigy.net.mx Miguel Palacios  mkpalacio@gmail.com
 * @date       jul 04, 2012
 * @version    1.0
 **/


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

void Navegacion::Main()
{
		/**
	 * \sa cambiar_estado
	 **/
	std::string accion;
	Location temp;
	CvPoint center;
	std::list<ArPose> path;
	
	Gui::getInstance();
	pluginTab = new Tab("Navegacion");
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
	

	robotController.setOdometer(patrol->getInstance().Position.get_X()*10, patrol->getInstance().Position.get_Y()*-10, patrol->getInstance().Position.get_Angle());
	
	for (;;)
	{
 		accion=patrol->getInstance().get_Action();
 		temp.set(robotController.getRobotPositionX()/10, (robotController.getRobotPositionY()/10)*-1, robotController.getRobotHeading());
 		patrol->getInstance().setPosition(temp);
		if (accion=="entrar")
		{
			print(logTerminal,"Iniciando: entrar\n");
			printf("iniciando entrar");
			do {}
			while (robotController.isThereObstacle(1000));
			ArPose position(robotController.getRobotPositionX()-1500,robotController.getRobotPositionY(),0);
			path.clear();
			path.push_back(position);
			robotController.setPath(path);
			robotController.moveRobot();
			patrol->getInstance().set_Action(cambiar_estado("dentro_arena", "si"));
			print(logTerminal,"Terminado: entrar\n");
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
			    ArPose position((*list_iter).get_X()*10,(*list_iter).get_Y()*-10,(*list_iter).get_Angle());
			    path.push_back(position);
			    
			  }
			  robotController.setPath(path);
			  robotController.moveRobot();
			}
			patrol->getInstance().Clear_path();
			patrol->getInstance().set_Action(cambiar_estado("destino_alcanzado","si"));
			print(logTerminal,"Terminado: navegar_destino\n");
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
			  patrol->getInstance().set_Action(cambiar_estado("persona_aproximada", "si"));
			print(logTerminal,"Terminado: aproximar_persona\n");
			}
			sleep(1);
		}
	}
}

void Navegacion::run()
{
	pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}



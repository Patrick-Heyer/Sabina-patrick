#include <iostream>
#include <pthread.h>
#include <vector>

#include "../Gui/gui.h"
#include "../Gui/tab.h"
#include "../Gui/console.h"
#include "../Plugin_API/pluginapi.h"
#include "../Shared_memory/Robot.h"

#include "armkatana.h"
#include "armkatanaforsabina.h"

#include "../MDP/MDP.h"


Robot *patrol;
Tab *pluginTab;
Console *logTerminal;

#define PLUGIN_NAME "Manipulacion"

/**
 * @brief Manipulacion plugin
 * 
 * This plugin is used for object manipulation using the Katana 6M arm.
 * @author     Patrick Heyer, patrickhey@prodigy.net.mx, Irving V&aacute;squez ivasquez@inaoep.mx
 * @date       jul 04, 2012
 * @version    1.0
 **/
class Manipulacion : public IPlugin
{      
public:
	void Main();
	void run();
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
	return new Manipulacion;
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

void Manipulacion::Main()
{
	/**
	 * \sa cambiar_estado
	 **/
	Gui::getInstance(); 
	pluginTab = new Tab("Manipulacion"); 
	logTerminal = new Console(0,HEIGHT*.02,500,HEIGHT/2,"error", pluginTab);
	ArmKatanaForSabina brazo;
	brazo.init( "192.168.168.232", "../data/configfiles450/katana6M180_G.cfg");
	sleep(5);    
	brazo.moveToCarriyingPos();
	sleep(5);
	brazo.closerGripper();
	sleep(5);
	brazo.moveToNavigationPos();
	sleep(5);	
	
	std::string accion;
	for (;;)
	{
		if(accion=="sujetar_objeto")
		{
			Gui::getInstance().setActiveTab("Manipulacion");
			patrol->getInstance().Sintetizer.set_Phrase("please put the object in front of me in my gripper");
			sleep(10);
			brazo.openGripper();
			sleep(5);
			brazo.moveToNavigationPos();
			patrol->getInstance().set_Current_destination(patrol->getInstance().prev_destination->c_str());
			patrol->getInstance().set_Action(cambiar_estado("objeto_sujetado","si"));
			
		}
		
		accion=patrol->getInstance().get_Action();
		if(accion=="entregar_objeto")
		{
			Gui::getInstance().setActiveTab("Manipulacion");
			patrol->getInstance().Sintetizer.set_Phrase("Please take the objet of my gripper");
			brazo.moveToCarriyingPos();
			sleep(10);
			brazo.openGripper();
			sleep(5);
			brazo.moveToNavigationPos();
			patrol->getInstance().set_Action(cambiar_estado("objeto_entregado","si"));
			
		}
	}
}

void Manipulacion::run()
{
	pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}



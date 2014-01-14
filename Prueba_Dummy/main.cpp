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

#include <iostream>
#include <pthread.h>

#include "../Gui/gui.h"
#include "../Gui/console.h"
#include "../Gui/video.h"
#include "../Gui/tab.h"
#include "../Gui/input_singleton.h"
#include "../Plugin_API/pluginapi.h"
#include "../Shared_memory/Robot.h"

#include "../MDP/MDP.h"

Robot *patrol;
Tab *pluginTab;

#define PLUGIN_NAME "Prueba_Dummy"

/**
 * @brief Prueba_Dummy plugin
 * 
 * This plugin is used to perform simple actions not performed by other plugins (basicly a placeholder for actions with no defined plugin)
 * @author     Patrick Heyer, patrickhey@prodigy.net.mx
 * @date       jul 04, 2012
 * @version    1.0
 **/

class Prueba_Dummy : public IPlugin
{
public:
	void Main();
	void run();
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
	return new Prueba_Dummy;
}

PLUGIN_FUNC void DestroyPlugin(IPlugin *r)
{
	delete r;
}

PLUGIN_DISPLAY_NAME(PLUGIN_NAME);

PLUGIN_INIT()
{
	// register our new plugin
	std::cout << "PLUGIN_INIT" << std::endl;
	RegisterPlugin(PLUGIN_NAME, CreatePlugin, DestroyPlugin);
	return 0;
}

void Prueba_Dummy::Main()
{
	/**
	 * \sa cambiar_estado
	 **/
	Gui::getInstance();
	pluginTab = new Tab("Prueba_Dummy");
	Console *error_terminal;
	error_terminal = new Console(0,HEIGHT*.02,500,HEIGHT/2,"error", pluginTab);
	
	
	int step=0;
	int sali=0;
	std::string accion;
	
/*	   
    patrol->getInstance().set_Current_destination("LIVING");
	cambiar_estado("iniciado", "si");
	cambiar_estado("localizado", "si");
	cambiar_estado("dentro_arena", "si");
	cambiar_estado("detectada_p","si");
	cambiar_estado("pidiendo_p","si");
	cambiar_estado("nombre_pedido","si");
	cambiar_estado("aprendidas_p","dos");
	cambiar_estado("ruta_planeada", "si");
	cambiar_estado("destino_alcanzado","si");
	cambiar_estado("encontradas_p","si");

	patrol->getInstance().set_Action(cambiar_estado("tiene_destino","si"));
	*/
	Gui::getInstance().setActiveTab("polo");
	for (;;)
	{
		accion=patrol->getInstance().get_Action();
		if (accion=="pedir_persona")
		{
			patrol->getInstance().Sintetizer.set_Phrase("Hello i am sabina I am waiting for a user ");
			sleep(2);
			patrol->getInstance().set_Action(cambiar_estado("pidiendo_p","si"));
		}
		
		if (accion=="dar_presentacion")
		{
			patrol->getInstance().Sintetizer.set_Phrase("Hello i am sabina and this is my presentation I am a service robot designed in puebla mexico by the team Markovito from the INAOE ");
			sleep(20);
			patrol->getInstance().set_Action(cambiar_estado("presentacionDada","si"));
		}
		
		if (accion=="actualizar")
		{
			if (step==0)
			{
				patrol->getInstance().Sintetizer.set_Phrase("Updating");
				sleep(5);
				patrol->getInstance().set_Current_destination("EXIT");
				
				cambiar_estado("ruta_planeada","no");
				patrol->getInstance().set_Action(cambiar_estado("destino_alcanzado","no"));
				sleep(2);
				step++;
			}
			else
			{
				patrol->getInstance().Sintetizer.set_Phrase("i will be back");
				sleep(4);
				patrol->getInstance().Sintetizer.set_Phrase("im waiting for my team");
				for (;;)
				{
				}
			}
		}
		
		if (accion=="aproximar_objeto")
		{
			
			patrol->getInstance().set_Action(cambiar_estado("objeto_aproximado", "si"));
			
		}
		
		if (accion=="salir")
		{
			if(sali==0)
			{
				patrol->getInstance().Sintetizer.set_Phrase("sorry i think im runing out of time i will go to the exit now");
		sleep(5);
		patrol->getInstance().set_Current_destination("EXIT");
		cambiar_estado("ruta_planeada","no");
		patrol->getInstance().set_Action(cambiar_estado("destino_alcanzado","no"));
		sali++;
			}
			else
			{
				patrol->getInstance().Sintetizer.set_Phrase("i am out now i hope i made it on time");
		sleep(10);
		for(;;);
			}
		}
		
	}
	
}

void Prueba_Dummy::run()
{
	pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}



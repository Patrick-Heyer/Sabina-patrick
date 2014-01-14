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

#include <sys/stat.h>
#include <sys/types.h>
#include <ctime>

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
	void stop();
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

int entregas=0;
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
	
	   
    patrol->getInstance().set_Current_destination("LIVING");
	
	string prev("");
	
	time_t end,now;
	do{
	}while(patrol->getInstance().Main_system!=true);
	end=time(NULL);
	struct tm *polo=localtime(&end);
	polo->tm_min+=patrol->getInstance().test_time-patrol->getInstance().tiempo_salida;
	polo->tm_sec+=0;
	end = mktime (polo);
	
bool first=true;
	
	for (;;)
	{
		
		time (&now);
		patrol->getInstance().test_time=difftime(end,now);
// 		if(difftime(end,now)<=0 && patrol->getInstance().get_Current_destination()!="EXIT" )
// 		{
// 			patrol->getInstance().Sintetizer.set_Phrase("excuse me i am going for your medicine");
// 			sleep(7);
// 			patrol->getInstance().set_Current_destination("EXIT");
// 			cambiar_estado("ruta_planeada", "no");
// 			patrol->getInstance().set_Action(cambiar_estado("destino_alcanzado", "no"));
// 		}
// 		

		accion=patrol->getInstance().get_Action();
		if(accion!=prev)
		{
			std::cout << accion << std::endl;
			prev=accion;
		}		
		

		
/*
		if (accion=="dar_presentacion")
		{
			patrol->getInstance().Sintetizer.set_Phrase("Hello i am sabina and this is my presentation I am a service robot designed in puebla mexico by the team Markovito from the INAOE ");
			sleep(5);
			patrol->getInstance().set_Action(cambiar_estado("presentacionDada","si"));
		}
		 */
		if(accion=="detectar_incendio" && first)
		{
			system("./Humo/humo & ");
			first=false;
		}
		/*
		if(accion=="entregar_objeto")
		{
			patrol->getInstance().Sintetizer.set_Phrase("Please take the objet of my gripper");
			sleep(3);
			patrol->getInstance().set_Action(cambiar_estado("objeto_entregado","si"));	
		}	
		
		if(accion=="sujetar_objeto")
		{
			patrol->getInstance().Sintetizer.set_Phrase("i think i cant get the object");
			sleep(3);
			patrol->getInstance().set_Current_destination("LIVING");
			cambiar_estado("ruta_planeada", "no");
			cambiar_estado("destino_alcanzado", "no");
			
			patrol->getInstance().set_Action(cambiar_estado("objeto_sujeto","si"));
		}	

		if(accion=="entregar_gripper")
		{
			patrol->getInstance().Sintetizer.set_Phrase("Please take the rehabilitation device");
			sleep(10);
			patrol->getInstance().set_Action(cambiar_estado("entregado_gripper","si"));
		}
*/
		if(accion=="finalizar_orden")
		{
			patrol->getInstance().set_Current_destination("EXIT");
			cambiar_estado("ruta_planeada", "no");
			cambiar_estado("destino_alcanzado", "no");
			patrol->getInstance().set_Action(cambiar_estado("orden_finalizada","si"));
		}
/*		
		if(accion=="salir_arena")
		{
			exit(0);
		}
		
		if(accion=="aprender_pedido_R")
		{
			patrol->getInstance().Sintetizer.set_Phrase("PERFECT");
			std::cout << "perfect" << std::endl;
			sleep(30);
			patrol->getInstance().set_Current_destination("TABLE 3");
			patrol->getInstance().set_Action(cambiar_estado("orden_pedida", "si"));
			
		}
		
		if(accion=="pedir_paciente")
		{
			patrol->getInstance().Sintetizer.set_Phrase("Hello i am waiting for mister DAVID");
			patrol->getInstance().set_Action(cambiar_estado("pidiendo_p", "si"));
		}
	
		if(accion=="localizar")
		{
			patrol->getInstance().set_Action(cambiar_estado("localizado", "si"));
		}
		
		if(accion=="hacer_show")
		{
			patrol->getInstance().Sintetizer.set_Phrase("my records show tha this is your first time");
			sleep(5);
			patrol->getInstance().Sintetizer.set_Phrase("please calibrate te gripper as the doctor showed you");
			sleep(20);
			patrol->getInstance().Sintetizer.set_Phrase("now execute the game");
			sleep(25);
			patrol->getInstance().Sintetizer.set_Phrase("you are doing good for your first time");
			sleep(10);
			patrol->getInstance().Sintetizer.set_Phrase("did you know tha this sistem was developd in mexico");
			sleep(10);
			patrol->getInstance().Sintetizer.set_Phrase("we hope in the future you can do this therapy at alone");
			sleep(15);
			patrol->getInstance().Sintetizer.set_Phrase("great job");
			sleep(15);
			patrol->getInstance().Sintetizer.set_Phrase("Very Nice");
			sleep(15);
			patrol->getInstance().Sintetizer.set_Phrase("excelente");
			sleep(15);
			patrol->getInstance().Sintetizer.set_Phrase("excelente");
			sleep(15);
		}
		*/
	}
	
}

void Prueba_Dummy::run()
{
	pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}

void Prueba_Dummy::stop()
{
	
}

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

#include "../MDP/MDP.h"

Robot *patrol;
tab *current_tab;

#define PLUGIN_NAME "Dummy"

class Dummy : public IPlugin
{
public:
    void Main();
    void run();
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Dummy;
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


void Dummy::Main()
{

    GUI::GetInstance();
    current_tab = new tab("Dummy");
    console *error_terminal;
    error_terminal = new console(0,HEIGHT*.02,500,HEIGHT/2,"error", current_tab);


//    patrol->get_Instance().set_Action(cambiar_estado("iniciado", "si"));
//             patrol->get_Instance().set_Current_destination("DINNING");
// 	cambiar_estado("localizado", "si");
// 	cambiar_estado("dentro_arena", "si")
// 	cambiar_estado("detectada_p","si");
// 	cambiar_estado("pidiendo_p","si");
// 	cambiar_estado("nombre_pedido","si");
// 	cambiar_estado("aprendidas_p","dos");
// 	cambiar_estado("ruta_planeada", "si");
// 	cambiar_estado("destino_alcanzado","si");

// 	patrol->get_Instance().set_Action(cambiar_estado("tiene_destino","si"));

    int step=0;
    int sali=0;
    std::string accion;
    for (;;)
    {
        accion=patrol->get_Instance().get_Action();
        if (accion=="pedir_persona")
        {
            patrol->get_Instance().Sintetizer.set_Phrase("Hello i am sabina I am waiting for a user ");
            sleep(2);
            patrol->get_Instance().set_Action(cambiar_estado("pidiendo_p","si"));
        }
        if (accion=="dar_presentacion")
        {
            patrol->get_Instance().Sintetizer.set_Phrase("Hello i am sabina and this is my presentation I am a service robot designed in puebla mexico by the team Markovito from the INAOE ");
            sleep(20);
            patrol->get_Instance().set_Action(cambiar_estado("presentacionDada","si"));
        }
        if (accion=="entregar_objeto")
        {
            patrol->get_Instance().Sintetizer.set_Phrase("Please take the objetct from my greeper ");
            sleep(5);
            patrol->get_Instance().set_Action(cambiar_estado("objeto_entregado","si"));
        }

        if (accion=="actualizar")
        {
            if (step==0)
            {
                patrol->get_Instance().Sintetizer.set_Phrase("Updating");
                sleep(5);
                patrol->get_Instance().set_Current_destination("EXIT");

                cambiar_estado("ruta_planeada","no");
                patrol->get_Instance().set_Action(cambiar_estado("destino_alcanzado","no"));
                sleep(2);
                step++;
            }
            else
            {
                patrol->get_Instance().Sintetizer.set_Phrase("i will be back");
                sleep(4);
                patrol->get_Instance().Sintetizer.set_Phrase("im waiting for my team");
                for (;;)
                {
                }
            }
        }
        if (accion=="aproximar_personas")
        {
            patrol->get_Instance().set_Action(cambiar_estado("persona_aproximada", "si"));
        }
        
        if (accion=="aproximar_objeto")
        {

             patrol->get_Instance().set_Action(cambiar_estado("objeto_aproximado", "si"));
           
        }
        
        if (accion=="salir")
        {
	  if(sali==0)
	  {
		patrol->get_Instance().Sintetizer.set_Phrase("sorry i think im runing out of time i will go to the exit now");
		sleep(5);
		patrol->get_Instance().set_Current_destination("EXIT");
                cambiar_estado("ruta_planeada","no");
                patrol->get_Instance().set_Action(cambiar_estado("destino_alcanzado","no"));
		sali++;
	  }
	  else
	  {
		patrol->get_Instance().Sintetizer.set_Phrase("i am out now i hope i made it on time");
		sleep(10);
		for(;;);
	  }
        }


    }

}

void Dummy::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}



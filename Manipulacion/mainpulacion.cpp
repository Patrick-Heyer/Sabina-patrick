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

#include <iostream>
//#include "treeconfiguration.h"
//#include "plannerrrt.h"
//#include "plannerkatana.h"
#include "armkatana.h"
#include "armkatanaforsabina.h"
#include <vector>

#include "../MDP/MDP.h"

Robot *patrol;
tab *current_tab;

#define PLUGIN_NAME "Manipulacion"

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

void Manipulacion::Main()
{

    GUI::GetInstance();
    current_tab = new tab("Manipulacion");
    console *error_terminal;
    error_terminal = new console(0,HEIGHT*.02,500,HEIGHT/2,"error", current_tab);
    // ArmKatanaForSabina brazo;
    std::cout << "antes de init: ";
    //  brazo.init();
//   brazo.moveToCarriyingPos();
    sleep(2);
//   brazo.closerGripper();


    std::string accion;
    for (;;)
    {
        accion=patrol->get_Instance().get_Action();
        if(accion=="entregar_objeto")
        {
            GUI::GetInstance().Set_Active_Tab("Manipulacion");
            patrol->get_Instance().Sintetizer.set_Phrase("Please take the objet of my gripper");
            sleep(10);
            //  brazo.openGripper();
            sleep(5);
            // brazo.moveToNavigationPos();
            patrol->get_Instance().set_Action(cambiar_estado("objeto_entregado","si"));

        }

        if(accion=="sujetar_objeto")
        {
            GUI::GetInstance().Set_Active_Tab("Manipulacion");
            patrol->get_Instance().Sintetizer.set_Phrase("please put the object in front of me in my gripper");
            sleep(10);
            //  brazo.openGripper();
            sleep(5);
            // brazo.moveToNavigationPos();
            patrol->get_Instance().set_Current_destination(patrol->get_Instance().prev_destination->c_str());

            cambiar_estado("ruta_planeada", "no");
            cambiar_estado("destino_alcanzado","no");
            patrol->get_Instance().set_Action(cambiar_estado("objeto_sujetado","si"));

        }
    }
    //brazo.calibrate();
//     while( true ){
//
// 	std::cout << "Valor de X: ";
// 	std::cin>>x;
// 	std::cout << "Valor de Y: ";
// 	std::cin>>y;
// 	std::cout << "Valor de Z: ";
// 	std::cin>>z;
//
// 	brazo.graspObjectAt(x, y, z);
//     }
//     std::cout << "Done!" << std::endl;

}

void Manipulacion::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}



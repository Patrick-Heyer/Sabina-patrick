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
#include "../Human/Human.h"

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
    do {
    } while(patrol->getInstance().Main_system!=true);
    end=time(NULL);
    struct tm *polo=localtime(&end);
    polo->tm_min+=patrol->getInstance().test_time-patrol->getInstance().exit_time;
    polo->tm_sec+=0;
    end = mktime (polo);


    bool first=true;


    for (;;)
    {
        time (&now);
        patrol->getInstance().test_time=difftime(end,now);
        /*
        if(difftime(end,now)<=0 && patrol->getInstance().get_Current_destination()!="EXIT" )
        {
        	patrol->getInstance().Sintetizer.set_Phrase("excuse me i am out off time");
        	sleep(7);
        	patrol->getInstance().set_Current_destination("EXIT");
        	cambiar_estado("ruta_planeada", "no");
        	patrol->getInstance().set_Action(cambiar_estado("destino_alcanzado", "no"));
        }
        */
        accion=patrol->getInstance().get_Action();
        if(accion!=prev)
        {
            std::cout << accion << std::endl;
            prev=accion;
        }
        if(accion=="aprender_pedido_R")
        {
            skeletonJoint R_Hand, L_Hand, Torso;
            std::map<std::string, skeletonJoint>::iterator iter;
            do {
                for ( iter=Human::getInstance().Skeleton->begin(); iter !=Human::getInstance().Skeleton->end(); ++iter )
                {
                    if ( iter->second.Get_name()=="TORSO" ) Torso=iter->second;
                    if ( iter->second.Get_name()=="LEFT_HAND" ) L_Hand=iter->second;
                    if ( iter->second.Get_name()=="RIGHT_HAND" ) R_Hand=iter->second;
                }

            } while(R_Hand.Get_y() <Torso.Get_y() && L_Hand.Get_y() <Torso.Get_y());

            if ( R_Hand.Get_y() >Torso.Get_y() )
            {
                patrol->getInstance().set_Current_destination("CHECK1");
                patrol->getInstance().Sintetizer.set_Phrase("i will go to check point ONE");
                sleep(2);
            }

            if ( L_Hand.Get_y() >Torso.Get_y() )
            {
                patrol->getInstance().set_Current_destination("CHECK2");
                patrol->getInstance().Sintetizer.set_Phrase("i will go to check point TWO");
                sleep(2);
            }


            sleep(5);
            patrol->getInstance().set_Action("none");
        }

    }

}

void Prueba_Dummy::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}

void Prueba_Dummy::stop()
{

}

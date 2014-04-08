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
#include "../Gui/tab.h"

#include "../Plugin_API/pluginapi.h"
#include "../Shared_memory/Robot.h"


#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <ctime>

Robot *patrol;
Tab *pluginTab;

#define PLUGIN_NAME "Sequencer"

/**
 * @brief Sequencer plugin
 *
 * This plugin is used to perform simple actions not performed by other plugins (basicly a placeholder for actions with no defined plugin)
 * @author     Patrick Heyer, patrickhey@prodigy.net.mx
 * @date       jul 04, 2012
 * @version    1.0
 **/

class Sequencer : public IPlugin
{
public:
    void Main();
    void stop();
    void run();
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Sequencer;
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

int textos=1;
void Sequencer::Main()
{
    /**
     * \sa cambiar_estado
     **/
    Gui::getInstance();
    pluginTab = new Tab("Sequencer");

    std::ifstream inFile;


    inFile.open(patrol->getInstance().seq_path->c_str());
    if (!inFile) {
        std::cout << "Unable to open file" <<std::endl;

    }

    std::string accion;

    while (inFile >> accion)
    {        
            patrol->getInstance().set_Action(accion);
	    accion=patrol->getInstance().get_Action();
	    std::cout << "Accion in sequence " << patrol->getInstance().get_Action() << std::endl;
	    
	    if(accion=="read_text")
	    {
	      std::stringstream ss;
	      ss.str("");
	      ss << "festival --tts ../data/Sequencer/Texto_" << textos;
	      system(ss.str().c_str());
	      textos++;
	      patrol->getInstance().set_Action("none");
	      
	    }
	     if(accion=="set_next_destination")
	    {
	      std::string destination;
	      inFile >> destination;
	      patrol->getInstance().set_Current_destination(destination.c_str());
	      std::cout << patrol->getInstance().get_Current_destination() << std::endl;
	      patrol->getInstance().set_Action("none");
	      
	    }
	    if(accion=="set_next_object")
	    {
	      std::string objectr;
	      inFile >> objectr;
	      patrol->getInstance().Microphone.set_Phrase(objectr.c_str());
	      std::cout << patrol->getInstance().Microphone.get_Phrase() << std::endl;
	      patrol->getInstance().set_Action("none");
	    }
	      
	    if(accion=="wait")
	    {
	     sleep(5);
	      patrol->getInstance().set_Action("none");
	      
	    }
	do
        {
	  usleep(500);
        } while(patrol->getInstance().get_Action()!="none");
    }
    inFile.close();


}

void Sequencer::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}

void Sequencer::stop()
{

}

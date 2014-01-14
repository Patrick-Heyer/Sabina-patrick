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
#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include "../Gui/gui.h"
#include "../Gui/video.h"
#include "../Plugin_API/pluginapi.h"
#include "../Shared_memory/Robot.h"

#include "../Gui/input_singleton.h"
#include "../Gui/gui.h"
#include "../Gui/tab.h"
#include "../Gui/console.h"
#include "../Gui/video.h"

Robot *patrol;

Console *cons;
Tab *pluginTab;
InputSingleton *teclado;

#include <opencv/cv.h>
#include <opencv/highgui.h>

#define PLUGIN_NAME "Voice_Synthesis"

/**
 * @brief Voice_Synthesis plugin
 * 
 * This plugin uses a system call to festival to synthesize voice. (To modify the voice change festivals system settings).
 * @author     Patrick Heyer, patrickhey@prodigy.net.mx
 * @date       jul 04, 2012
 * @version    1.0
 **/
class Voice_Synthesis : public IPlugin
{
public:
    void Main();
    void run();
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Voice_Synthesis;
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

void Voice_Synthesis::Main()
{
    Gui::getInstance();
    pluginTab = new Tab("Voice_Synthesis");
    cons= new Console(WIDTH/2,HEIGHT*.02,WIDTH/2,HEIGHT/2, "Error", pluginTab);
    while (true)
    {

        if (patrol->getInstance().Sintetizer.get_Phrase()!="")
        {
        std::stringstream ss;
        ss << "echo  '" << patrol->getInstance().Sintetizer.get_Phrase() << "' | festival --tts";
        std::string temp=ss.str();
        std::cout <<temp << std::endl;
        system (temp.c_str());
	
        patrol->getInstance().Sintetizer.set_Phrase("");
        }
        
    }
    
}


void Voice_Synthesis::run()
{    
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}



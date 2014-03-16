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
#include <map>
#include <pthread.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include "../Gui/gui.h"
#include "../Gui/video.h"
#include "../Plugin_API/pluginapi.h"
#include "../Shared_memory/Robot.h"
#include "../Shared_memory/Location.h"

#include "../Gui/input_singleton.h"
#include "../Gui/gui.h"
#include "../Gui/tab.h"
#include "../Gui/console.h"

#include "../MDP/MDP.h"

#include <cstdlib>
#include <iostream>

#include "Thread.h"
#include "NetThread.h"
#include "gsNetPoll_g++.h"


Robot *patrol;

Video *videoDisplay;
Tab *pluginTab;
Console *cons;

#define PLUGIN_NAME "Net_server"

/**
 * @brief Net_server plugin
 *
 * This plugin is a network server that accepts client conections to comunicate with other programs (running on the same computer or over the network)
 * @author     Patrick Heyer, patrickhey@prodigy.net.mx, Juan Herrera juan_antonio_@hotmail.com, Manuel Oropeza zodiacanimations@msn.com
 * @date       jul 04, 2012
 * @version    1.0
 **/
class Net_server : public IPlugin
{
public:
    void Main();
    void stop();
    void run();
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Net_server;
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

int contao=0;

void Net_server::Main()
{
    /**
     * \sa print
     **/
    Gui::getInstance();
    pluginTab = new Tab("Net_server");
    cons= new Console(WIDTH-600,HEIGHT*.02,600,HEIGHT/2, "net_console", pluginTab);

    NetThread *net = new NetThread();


    patrol->getInstance().Microphone.set_Words("");

    net->SetOutputStream(stdout);
    net->OpenReadPort(2070);
    net->start();
    net->Accept();
    sleep(5);

    std::string accion;

    for(;;)
    {
        accion= patrol->getInstance().get_Action();

        if(accion=="")
        {
            net->Read();
            if(net->GetStatus()>0)
            {
                string s = net->GetIncoming();
                cout << s << endl;
                sleep(1);
                net->messages.clear();
                if (s.find("String to find")!=s.npos)
                {
                }
            }

        }



    }

}



void Net_server::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}

void Net_server::stop()
{

}



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

#include <cstdlib>
#include <iostream>

#include "Thread.h"
#include "NetThread.h"
#include "gsNetPoll_g++.h"


Robot *patrol;

Video *videoDisplay;
Tab *pluginTab;
Console *cons;

#define PLUGIN_NAME "Net_client"

/**
 * @brief Net_client plugin
 *
 * This plugin is a network server that accepts client conections to comunicate with other programs (running on the same computer or over the network)
 * @author     Patrick Heyer, patrickhey@prodigy.net.mx, Juan Herrera juan_antonio_@hotmail.com, Manuel Oropeza zodiacanimations@msn.com
 * @date       jul 04, 2012
 * @version    1.0
 **/
class Net_client : public IPlugin
{
public:
    void Main();
    void stop();
    void run();
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Net_client;
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
int lugar=1;

void Net_client::Main()
{

    Gui::getInstance();
    pluginTab = new Tab("Net_client");
    cons= new Console(WIDTH-600,HEIGHT*.02,600,HEIGHT/2, "net_console", pluginTab);

    char message[1024];

    NetThread *net = new  NetThread();
    net->SetOutputStream(stdout);
    sleep(5);
    net->OpenOutputAddress("localhost", 2070);
    int x=0;
    string accion;
    while(1)
    {
        accion= patrol->getInstance().get_Action();
        if(accion=="aprender_nombre")
        {
            net->Write("LINDA");
            sleep(1);
            net->Read();
            if(net->GetStatus()>0)
            {
                string s = net->GetIncoming();

            }

            sleep(1);
        }

        if(accion=="encontrar_persona")
        {
            net->Write("YES");
            sleep(1);
            net->Read();
            if(net->GetStatus()>0)
            {
                string s = net->GetIncoming();

            }
            sleep(1);
        }

        if(accion=="aprender_orden")
        {
            net->Write("COCA");
            sleep(1);
            net->Read();
            if(net->GetStatus()>0)
            {
                string s = net->GetIncoming();

            }
            sleep(1);
        }

        if(accion=="aprender_mesa")
        {
            std::stringstream ss;
            ss<<"TABLE " << lugar;
            net->Write(ss.str().c_str());
            sleep(1);
            net->Read();
            if(net->GetStatus()>0)
            {
                string s = net->GetIncoming();

            }
            sleep(1);
            lugar++;
        }

        if(accion=="aprender_drinks")
        {
            std::stringstream ss;
            ss<<"DRINKS " ;
            net->Write(ss.str().c_str());
            sleep(1);
            net->Read();
            if(net->GetStatus()>0)
            {
                string s = net->GetIncoming();

            }
            sleep(1);
        }


        if(accion=="aprender_snaks")
        {
            std::stringstream ss;
            ss<<"SNAKS ";
            net->Write(ss.str().c_str());
            sleep(1);
            net->Read();
            if(net->GetStatus()>0)
            {
                string s = net->GetIncoming();

            }
            sleep(1);
        }
        net->messages.clear();
        sleep(1);
        net->messages.clear();
    }

}

void Net_client::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}

void Net_client::stop()
{

}


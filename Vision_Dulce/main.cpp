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

#include "../Plugin_API/pluginapi.h"
#include "../Shared_memory/Robot.h"
#include "../MDP/MDP.h"
#include "../Gui/gui.h"
#include "../Gui/console.h"
#include "../Gui/video.h"


#include <cstdlib>
#include <iostream>
#include <fstream>



using namespace cv;
using namespace std;


Robot *patrol;
Tab *pluginTab;

#define PLUGIN_NAME "Vision_Dulce"


class Vision_Dulce : public IPlugin
{
public:
    void Main();
    void stop();
    void run();
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Vision_Dulce;
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


bool first=true;
void Vision_Dulce::Main()
{
    /**
     * \sa cambiar_estado
     **/

    Gui::getInstance();
    patrol->getInstance().Microphone.set_Phrase("COFFEE");
    pluginTab = new Tab("Vision_Dulce");
    String accion;
    std::ifstream infile3("../data/Objects");
    std::vector<string> objects;
    string line;
    while (getline(infile3, line))
    {
        objects.push_back(line);
    }
    for(;;)
    {
        accion= patrol->getInstance().get_Action();
        if(accion=="reconocer_objeto" && first)
        {
            first=false;
            for (int i=0; i<objects.size(); i++) {

                if (patrol->getInstance().Microphone.get_Phrase().find((objects[i]))!=patrol->getInstance().Microphone.get_Phrase().npos)
                {
                    std::stringstream ss;
                    ss << "./Vision_Dulce_ejecutable/vision_dulce_ejecutable " << objects[i] << " &";
                    cout << ss.str().c_str() << endl;
                    system(ss.str().c_str());
                    sleep(60);
                }

                accion= patrol->getInstance().get_Action();
                if (accion!="reconocer_objeto")
                    break;
            }
        }


    }

}

void Vision_Dulce::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}

void Vision_Dulce::stop()
{

}


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

#include "surfrecognizer.h"

Robot *patrol;
Video *videoDisplay;
Console *logTerminal;
Tab *pluginTab;

#define PLUGIN_NAME "Vision_Surf"

/**
 * @brief Vision_Surf plugin
 * 
 * This plugin is used for object recognition based on OpenCV's basic SURF example
 * @author     Patrick Heyer, patrickhey@prodigy.net.mx, Irving V&aacute;squez ivasquez@inaoep.mx
 * @date       jul 04, 2012
 * @version    1.0
 **/
class Vision_Surf : public IPlugin
{
public:
    void Main();
    void run();
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Vision_Surf;
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


void Vision_Surf::Main()
{
	/**
	 * \sa cambiar_estado
	 **/

    Gui::getInstance();
    IplImage *frame;
    pluginTab = new Tab("Vision_Surf");
    videoDisplay = new Video(0,HEIGHT*.02,WIDTH/2,HEIGHT/2,"Vision_Surf", pluginTab);

    SurfRecognizer recognizer;
    //    SurfRecognizer recognizer2;
    //    SurfRecognizer recognizer3;

    // string objeto_file("box.png");
    std::stringstream ss;

    ss << "../data/Surf/" << "COCA" << ".png";
    string objeto_file1(ss.str());


    recognizer.setObjectFileName(objeto_file1.c_str());
    recognizer.setDisplaying(true);
    recognizer.setRecognitionTreshold(15);
    recognizer.loadObjectFile();

    std::string accion;

    for (;;)
    {
        accion=patrol->getInstance().get_Action();
        if (accion=="confirmar_objeto")
        {

            frame=patrol->getInstance().Ptz.get_image();
            videoDisplay->SetImage(frame);
            if (recognizer.recongnizeObjectInImage(frame))
            {
                patrol->getInstance().Sintetizer.set_Phrase("i found coca");
		sleep(3);
		patrol->getInstance().set_Action(cambiar_estado("objeto_confirmado", "si"));
            }
            sleep(1);
            cvReleaseImage(&frame);
            
        }
    }

}

void Vision_Surf::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}


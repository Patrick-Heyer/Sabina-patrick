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
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <string>

#include "../Gui/gui.h"
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

video *vid;
console *cons;
tab *current_tab;
Input_Singleton *teclado;

#define WIDTH 1440
#define HEIGHT 900

#define PLUGIN_NAME "Vision_Surf"

#include "surfrecognizer.h"

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

PLUGIN_FUNC void DestroyRenderer(IPlugin *r)
{
    delete r;
}

PLUGIN_DISPLAY_NAME(PLUGIN_NAME);

PLUGIN_INIT()
{
    // register our new renderer
    std::cout << "PLUGIN_INIT" << std::endl;
    RegisterPlugin(PLUGIN_NAME, CreatePlugin, DestroyRenderer);
    return 0;
}


void Vision_Surf::Main()
{


    GUI::GetInstance();
    IplImage *frame;
    current_tab = new tab("Vision_Surf");
    vid = new video(0,HEIGHT*.02,WIDTH/2,HEIGHT/2,"Vision_Surf", current_tab);

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
        accion=patrol->get_Instance().get_Action();
        if (accion=="confirmar_objeto")
        {

            frame=patrol->get_Instance().Ptz.get_image();
            vid->SetImage(frame);
            if (recognizer.recongnizeObjectInImage(frame))
            {
                patrol->get_Instance().Sintetizer.set_Phrase("i found coca");
		sleep(3);
		patrol->get_Instance().set_Action(cambiar_estado("objeto_confirmado", "si"));
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


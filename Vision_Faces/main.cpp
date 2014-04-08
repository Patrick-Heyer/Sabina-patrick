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
#include "../Shared_memory/Objective.h"

#include "../Gui/input_singleton.h"
#include "../Gui/gui.h"
#include "../Gui/tab.h"
#include "../Gui/console.h"
#include "../Gui/video.h"

#include "facedetect.h"
#include "load.h"
#include "sift.h"

Robot *patrol;
Tab *pluginTab;

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "../MDP/MDP.h"

#define PLUGIN_NAME "Vision_Faces"

/**
 * @brief Vision_Faces plugin
 *
 * This plugin is used to detect, learn and recognize human faces.
 * @author     Patrick Heyer, patrickhey@prodigy.net.mx
 * @date       jul 04, 2012
 * @version    1.0
 **/
class Vision_Faces : public IPlugin
{
public:
    void Main();
    void stop();
    void run();
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Vision_Faces;
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


void Vision_Faces::Main()
{

    /**
     * \sa detectFace
     * \sa learn
     * \sa recognize
     * \sa cambiar_estado
     **/
    int polo=0;

    Gui::getInstance();

    IplImage *frame;
    pluginTab = new Tab("Vision_Faces");

    initFaceDet("../data/haarcascade_frontalface_default.xml");
    initEyeDet("../data/parojos.xml");
    initEyeDetD("../data/ojoD.xml");
    initEyeDetI("../data/ojoI.xml");

    std::map<std::string, Objective>::iterator iter;

    std::string accion;
    for (;;)
    {
        accion=patrol->getInstance().get_Action();
        if (accion=="aprender_persona")
        {
            int aprendi;
            do
            {

                frame=patrol->getInstance().Ptz.get_image();
                aprendi=learn(frame);
            } while(aprendi==0);
	    std::stringstream ss;
	    ss.str("");
	    Objective temp;
	    temp.set_Name("");
	    temp.set_Visual_ID(aprendi);
	    temp.set_Type("person");
	    ss << temp.get_Name().c_str() << temp.get_Type()->c_str() << temp.get_Visual_ID();
	    std::cout << ss.str().c_str() << std::endl;
	    temp.set_Last_x(patrol->getInstance().get_Position().get_X());
	    temp.set_Last_y(patrol->getInstance().get_Position().get_Y());
	    patrol->getInstance().Objectives->insert(std::make_pair(ss.str().c_str(), temp));
            patrol->getInstance().set_Action("none");
        }

        if (accion=="reconocer_persona")
        {
            int reconocio;
            do
            {
                frame=patrol->getInstance().Ptz.get_image();
                reconocio=recognize(frame);
		

            } while(reconocio==0);
	    for(iter=patrol->getInstance().Objectives->begin(); iter !=patrol->getInstance().Objectives->end(); ++iter)
                {
		    if( iter->second.get_Visual_ID()==reconocio)
		    {
			std::cout << iter->first << std::endl;
		    }
		}
            patrol->getInstance().set_Action("none");

        }

        if (accion=="detectar_persona")
        {
            int detecto;
            do
            {
                frame=patrol->getInstance().Ptz.get_image();
                detectFace(frame,&detecto);
            } while(detecto<1);
            patrol->getInstance().set_Action("none");
        }

        cvReleaseImage(&frame);
    }

}

void Vision_Faces::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}

void Vision_Faces::stop()
{

}



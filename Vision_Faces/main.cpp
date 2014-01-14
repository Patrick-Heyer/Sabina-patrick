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
#include "recognize.h"
#include "learn.h"
#include "sift.h"

Robot *patrol;

Video *videoDisplay;
Console *cons;
Tab *pluginTab;
InputSingleton *teclado;

#define WIDTH 1366


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
    std::cout << "PLUGIN_INIT" << std::endl;
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
    videoDisplay = new Video(0,HEIGHT*.02,WIDTH/2,HEIGHT/2,"Vision_Faces", pluginTab);

    cons= new Console(WIDTH/2,HEIGHT*.02,WIDTH/2,HEIGHT/2, "Error", pluginTab);
    initFaceDet("../data/haarcascade_frontalface_default.xml");
    initEyeDet("../data/parojos.xml");
    initEyeDetD("../data/ojoD.xml");
    initEyeDetI("../data/ojoI.xml");




    std::map<std::string, Objective>::iterator iter;
    std::string name;

    std::string accion;
    for (;;)
    {
        accion=patrol->getInstance().get_Action();
        frame=patrol->getInstance().Ptz.get_image();

        if (accion=="detectar_personas")
        {
            //GUI::getInstance().Set_Active_Tab("Vision_Faces");
            if (detectFace(frame,&polo)!=NULL)
            {
                patrol->getInstance().Sintetizer.set_Phrase("hello there");
                sleep(5);
                patrol->getInstance().set_Action(cambiar_estado("detectada_p","si"));
            }
        }

        if (accion=="aprender_persona")
        {
            //GUI::getInstance().Set_Active_Tab("Vision_Faces");
            int aprendi=learn(frame);
            Objective *temp;


            patrol->getInstance().Objectives->find(patrol->getInstance().Microphone.get_Phrase())->second.set_Visual_ID(aprendi);



            if (aprendi==1 || aprendi==2 )
            {
                std::stringstream ss;
                ss << "Ok i learnd your face with the name " <<patrol->getInstance().Microphone.get_Phrase();
                patrol->getInstance().Sintetizer.set_Phrase(ss.str());
                sleep(5);
                patrol->getInstance().Sintetizer.set_Phrase("Please step away");
                sleep(10);
                cambiar_estado("detectada_p","no");
                cambiar_estado("pidiendo_p","no");
                cambiar_estado("nombre_pedido","no");
                patrol->getInstance().set_Action(cambiar_estado("aprendidas_p","no"));

            }
            if (aprendi==3)
            {
                std::stringstream ss;
                ss << "Ok i learnd your face with the name " << patrol->getInstance().Microphone.get_Phrase();
                patrol->getInstance().Sintetizer.set_Phrase(ss.str());
                sleep(5);
                patrol->getInstance().Sintetizer.set_Phrase("Please step away");
                sleep(10);
                cambiar_estado("aprendidas_p","dos");
                patrol->getInstance().set_Action(cambiar_estado("ruta_planeada","no"));
            }

        }
        if (accion=="encontrar_personas")
        {
            //GUI::getInstance().Set_Active_Tab("Vision_Faces");
            patrol->getInstance().Sintetizer.set_Phrase("i am looking for people");
            if (detectFace(frame,&polo)!=NULL)
            {
                patrol->getInstance().Sintetizer.set_Phrase("Hello there seames to be a person let me confirm");
                sleep(5);
                patrol->getInstance().set_Action(cambiar_estado("encontradas_p","si"));
            }
        }

        if (accion=="confirmar_persona")
        {
            //GUI::getInstance().Set_Active_Tab("Vision_Faces");
            if (detectFace(frame,&polo)!=NULL)
            {
                patrol->getInstance().Sintetizer.set_Phrase("Yes there is a person in front of me");
                sleep(5);
                patrol->getInstance().set_Action(cambiar_estado("persona_confirmada","si"));
            }
            else
            {
                patrol->getInstance().Sintetizer.set_Phrase("there is no person my misstake");
                sleep(5);
                patrol->getInstance().set_Action(cambiar_estado("encontradas_p","no"));
            }
        }

        if (accion=="reconocer_personas")
        {
            //GUI::getInstance().Set_Active_Tab("Vision_Faces");
            int reconocio=recognize(frame);
            if (reconocio==0)
            {
                patrol->getInstance().Sintetizer.set_Phrase("hello i dont remember you i will introduce myself after the test");
                sleep(10);
                patrol->getInstance().set_Action(cambiar_estado("reconocidas_p","si"));
            }
            else
            {
                string temp_name;
                map<string,Objective>::iterator iter;
                for ( iter = patrol->getInstance().Objectives->begin(); iter != patrol->getInstance().Objectives->end(); iter++ ) {
                    if (iter->second.get_Visual_ID()==reconocio)
                        temp_name=iter->first;
                }

                std::stringstream ss;

                ss << "hello " << temp_name << " how are you? ";
                patrol->getInstance().Sintetizer.set_Phrase(ss.str());
                sleep(2);

                *patrol->getInstance().Last_objective=temp_name;
		patrol->getInstance().set_Action(cambiar_estado("reconocidas_p","si"));
            }
        }
        videoDisplay->SetImage(frame);
        cvReleaseImage(&frame);
    }

}

void Vision_Faces::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}


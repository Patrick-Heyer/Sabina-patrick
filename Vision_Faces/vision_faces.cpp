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
#include "../Gui/kinect.h"

#include "facedetect.h"
#include "recognize.h"
#include "learn.h"
#include "sift.h"

Robot *patrol;

video *vid;
console *cons;
tab *current_tab;
Input_Singleton *teclado;

#define WIDTH 1366
#define HEIGHT 768

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "../MDP/MDP.h"

#define PLUGIN_NAME "Vision_Faces"


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


void Vision_Faces::Main()
{
    int polo=0;

    GUI::GetInstance();

    IplImage *frame;
    current_tab = new tab("Vision_Faces");
    vid = new video(0,HEIGHT*.02,WIDTH/2,HEIGHT/2,"Vision_Faces", current_tab);

    cons= new console(WIDTH/2,HEIGHT*.02,WIDTH/2,HEIGHT/2, "Error", current_tab);
    initFaceDet("../data/haarcascade_frontalface_default.xml");
    initEyeDet("../data/parojos.xml");
    initEyeDetD("../data/ojoD.xml");
    initEyeDetI("../data/ojoI.xml");




    std::map<std::string, Objective>::iterator iter;
    std::string name;

    std::string accion;
    for (;;)
    {
        accion=patrol->get_Instance().get_Action();
        frame=patrol->get_Instance().Ptz.get_image();

        if (accion=="detectar_personas")
        {
            GUI::GetInstance().Set_Active_Tab("Vision_Faces");
            if (detectFace(frame,&polo)!=NULL)
            {
                patrol->get_Instance().Sintetizer.set_Phrase("hello there");
                sleep(5);
                patrol->get_Instance().set_Action(cambiar_estado("detectada_p","si"));
            }
        }

        if (accion=="aprender_persona")
        {
            GUI::GetInstance().Set_Active_Tab("Vision_Faces");
            int aprendi=learn(frame);
            Objective *temp;


            patrol->get_Instance().Objectives->find(patrol->get_Instance().Microphone.get_Phrase())->second.set_Visual_ID(aprendi);



            if (aprendi==1 || aprendi==2 )
            {
                std::stringstream ss;
                ss << "Ok i learnd your face with the name " <<patrol->get_Instance().Microphone.get_Phrase();
                patrol->get_Instance().Sintetizer.set_Phrase(ss.str());
                sleep(5);
                patrol->get_Instance().Sintetizer.set_Phrase("Please step away");
                sleep(10);
                cambiar_estado("detectada_p","no");
                cambiar_estado("pidiendo_p","no");
                cambiar_estado("nombre_pedido","no");
                patrol->get_Instance().set_Action(cambiar_estado("aprendidas_p","no"));

            }
            if (aprendi==3)
            {
                std::stringstream ss;
                ss << "Ok i learnd your face with the name " << patrol->get_Instance().Microphone.get_Phrase();
                patrol->get_Instance().Sintetizer.set_Phrase(ss.str());
                sleep(5);
                patrol->get_Instance().Sintetizer.set_Phrase("Please step away");
                sleep(10);
                cambiar_estado("aprendidas_p","dos");
                patrol->get_Instance().set_Action(cambiar_estado("ruta_planeada","no"));
            }

        }
        if (accion=="encontrar_personas")
        {
            GUI::GetInstance().Set_Active_Tab("Vision_Faces");
            patrol->get_Instance().Sintetizer.set_Phrase("i am looking for people");
            if (detectFace(frame,&polo)!=NULL)
            {
                patrol->get_Instance().Sintetizer.set_Phrase("Hello there seames to be a person let me confirm");
                sleep(10);
                patrol->get_Instance().set_Action(cambiar_estado("encontradas_p","si"));
            }
        }

        if (accion=="confirmar_persona")
        {
            GUI::GetInstance().Set_Active_Tab("Vision_Faces");
            if (detectFace(frame,&polo)!=NULL)
            {
                patrol->get_Instance().Sintetizer.set_Phrase("Yes there is a person in front of me");
                sleep(5);
                patrol->get_Instance().set_Action(cambiar_estado("persona_confirmada","si"));
            }
            else
            {
                patrol->get_Instance().Sintetizer.set_Phrase("there is no person my misstake");
                sleep(5);
                patrol->get_Instance().set_Action(cambiar_estado("encontradas_p","no"));
            }
        }

        if (accion=="reconocer_personas")
        {
            GUI::GetInstance().Set_Active_Tab("Vision_Faces");
            int reconocio=recognize(frame);
            if (reconocio==0)
            {
                patrol->get_Instance().Sintetizer.set_Phrase("hello i dont remember you i will introduce myself after the test");
                sleep(10);
                patrol->get_Instance().set_Action(cambiar_estado("reconocidas_p","si"));
            }
            else
            {
                string temp_name;
                map<string,Objective>::iterator iter;
                for ( iter = patrol->get_Instance().Objectives->begin(); iter != patrol->get_Instance().Objectives->end(); iter++ ) {
                    if (iter->second.get_Visual_ID()==reconocio)
                        temp_name=iter->first;
                }

                std::stringstream ss;

                ss << "hello " << temp_name << " how are you? ";
                patrol->get_Instance().Sintetizer.set_Phrase(ss.str());
                sleep(2);

                *patrol->get_Instance().Last_objective=temp_name;
		patrol->get_Instance().set_Action(cambiar_estado("reconocidas_p","si"));
            }
        }
        vid->SetImage(frame);
        cvReleaseImage(&frame);
        frame=patrol->get_Instance().Ptz.get_image();
        vid->SetImage(frame);
        cvReleaseImage(&frame);
        frame=patrol->get_Instance().Ptz.get_image();
        vid->SetImage(frame);
        cvReleaseImage(&frame);
    }

}

void Vision_Faces::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}


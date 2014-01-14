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
#include "../Gui/video.h"
#include "../Gui/kinect.h"

#include "CAmbiente.h"
#include "TiposBasicos.h"
#include "CProbot.h"
#include "Constants.h"
#include "read_prm.h"

#include "../MDP/MDP.h"

Robot *patrol;

video *vid;
tab *current_tab;
console *cons;

#define WIDTH 1440
#define HEIGHT 900

#define PLUGIN_NAME "Navigation_Planing"

class Navigation_Planing : public IPlugin
{
public:
    void Main();
    void run();
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Navigation_Planing;
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

void Navigation_Planing::Main()
{
    GUI::GetInstance();
    current_tab = new tab("Navigation_Planing");
    vid = new video(0,HEIGHT*.02,WIDTH/2,HEIGHT/2,"Navigation_Planing", current_tab);
    cons= new console(WIDTH-600,HEIGHT*.02,600,HEIGHT/2, "Navigation_console", current_tab);

    IplImage *map;

    string filename;
    CAmbiente *ambiente_p_v = NULL;
    uu_PlanningParameters parametros_v;
    CProbot prm;

    parametros_v = prm.Parametros();
    ambiente_p_v = (CAmbiente*)prm.Ambiente();


    if (ambiente_p_v != NULL)
    {
        parametros_v.NodeNumber = 1200;
        parametros_v.NeighbourDistance = 80;
        parametros_v.NeighbourNumber = 1200/20;
        ambiente_p_v->setsizeRobot( 59 );
        parametros_v.StepSize = 59/2-1;
        parametros_v.SamplingMethod = PRM::GAUSSIAN_S;

        filename = patrol->get_Instance().Enviroment.get_Image_path();

        prm.setPlanningParametros(parametros_v);
        print(cons, "Mapa: %s Numero de nodos %d  Distancia entre nodos: %f Tamaño del robot %d \n",
              filename.c_str(), parametros_v.NodeNumber, parametros_v.NeighbourDistance, ambiente_p_v->sizeRobot() );
        if (parametros_v.SamplingMethod == PRM::GAUSSIAN_S )
            print(cons,"Tipo muestreo: Aleatorio Gaussiano\n");
        else
            print(cons,"Tipo muestreo: Aleatorio Uniforme\n");

        ambiente_p_v->setMapaFileName(&filename);
    }
    map=cvLoadImage(patrol->get_Instance().Enviroment.get_Image_path().c_str());
    CvPoint center;
    list< Location> puntos;
    bool polo=false;
    std::string accion;
    for (;;)
    {
        accion=patrol->get_Instance().get_Action();
        IplImage *temp;
        temp=cvCloneImage(map);

        if (patrol->get_Instance().get_Action()=="planear_ruta")
        {

            puntos.clear();
            patrol->get_Instance().Route->clear();
            patrol->get_Instance().Sintetizer.set_Phrase("i am planing my route");
            int initial_x, final_x;
            int initial_y, final_y;
            GUI::GetInstance().Set_Active_Tab("Navigation_Planing");
            //         if (Input_Singleton::GetInstance().key=='t')
            typedef std::map<string,Destination> StringFloatMap;
            StringFloatMap::iterator pos;

            initial_x= patrol->get_Instance().Position.get_X();
            initial_y= patrol->get_Instance().Position.get_Y();

            pos = patrol->get_Instance().Destinations->find(patrol->get_Instance().get_Current_destination());
            if (pos != patrol->get_Instance().Destinations->end() )
            {
                final_x= pos->second.get_Coordinate().get_X();
                final_y= pos->second.get_Coordinate().get_Y();

            }

            read_prm(&prm, initial_y, initial_x, final_y, final_x);
            Input_Singleton::GetInstance().key==NULL;

            puntos=patrol->get_Instance().get_Route();
            if (puntos.size()>1)
            {
                polo=true;
                for (std::list<Location>::iterator list_iter = puntos.begin();
                        list_iter != puntos.end(); list_iter++)
                {
                    print(cons,"%d  %d \n ", (*list_iter).get_X(), (*list_iter).get_Y());
                }
            }
            puntos=patrol->get_Instance().get_Route();
            if (puntos.size()>1)
            {
                center.x=initial_x;
                center.y=initial_y;
                cvCircle(temp, center, 2, CV_RGB(0, 255, 0), -1, 8, 0);
                center.x=final_x;
                center.y=final_y;
                cvCircle(temp, center, 2, CV_RGB(0, 0, 255), -1, 8, 0);

                for (std::list<Location>::iterator list_iter = puntos.begin();
                        list_iter != puntos.end(); list_iter++)
                {

                    center.x=(*list_iter).get_X();
                    center.y=(*list_iter).get_Y();
                    cvCircle(temp, center, 2, CV_RGB(255, 0, 0), -1, 8, 0);
                }
            }
            vid->SetImage(temp);
            cvReleaseImage(&temp);
            patrol->get_Instance().set_Action(cambiar_estado("ruta_planeada","si"));


        }
        else
        {
            puntos=patrol->get_Instance().get_Route();
            if (puntos.size()>1)
            {
                for (std::list<Location>::iterator list_iter = puntos.begin();
                        list_iter != puntos.end(); list_iter++)
                {
                    center.x=(*list_iter).get_X();
                    center.y=(*list_iter).get_Y();
                    cvCircle(temp, center, 2, CV_RGB(255, 0, 0), -1, 8, 0);
                }
            }
            center.x=patrol->get_Instance().Position.get_X();
            center.y=patrol->get_Instance().Position.get_Y();
            cvCircle(temp, center, 2, CV_RGB(0, 0,255), -1, 8, 0);
            vid->SetImage(temp);
            cvReleaseImage(&temp);
        }
    }
}

void Navigation_Planing::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}



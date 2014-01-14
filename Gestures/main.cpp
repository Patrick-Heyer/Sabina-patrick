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

#include "../Gui/gui.h"
#include "../Gui/video.h"
#include "../Plugin_API/pluginapi.h"
#include "../Shared_memory/Robot.h"

#include "../Gui/input_singleton.h"
#include "../Gui/gui.h"
#include "../Gui/tab.h"
#include "../Gui/console.h"
#include "../Gui/video.h"



#include <iostream>
#include <math.h>
#include "observations.h"
#include "dynamic_win.h"
#include "hmm.h"

using namespace std;


Robot *patrol;

Console *cons;
Tab *pluginTab;




#define PLUGIN_NAME "Gestures"


/**
 * @brief Gestures plugin
 *
 * This plugin is used to recognize hand gestures using OpenNi with a Kinect sensor
 * @author     Patrick Heyer, patrickhey@prodigy.net.mx, Angel Oca&ntilde;a ahop_289@hotmail.com
 * @date       jul 04, 2012
 * @version    1.0
 **/
class Gestures : public IPlugin
{
public:
    void Main();
    void stop();
    void run();
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Gestures;
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

std::string accion;

void Gestures::Main()
{
    Gui::getInstance();
    pluginTab = new Tab("Gestures");



    int n = 3; //Number of HMM's states
    int m = 32; //Number of symbols of outs
    int M = 5; //Number of gestures
    int t = 0; //Count the numbers of Windows
    int l = 0; //Start point for the new window
    int k = 0; //End point for the new window
    int i, j; //Indices de iteraciones
    bool band = false; //Decide when start one gesture
    bool skeleton = true; //Esto se cambia con el metodo que indica si existe o no esqueleto
    int delta = 10; //Size of grow of windows
    int bf = 4; //Begin frame
    int sf = 6; //Stop frame
    float threshold = 1; //Threshold of votes to begin the method
    int recog; //Indica cual ademan se reconocio

    double x_hand, y_hand, z_hand, x_neck, y_neck; //Esto es temporal

    //creating the hmm models. One by each gesture.
    hmm HMM1("../data/gestures/a1.txt","../data/gestures/b1.txt",n,m);
    hmm HMM2("../data/gestures/a2.txt","../data/gestures/b2.txt",n,m);
    hmm HMM3("../data/gestures/a3.txt","../data/gestures/b3.txt",n,m);
    hmm HMM4("../data/gestures/a4.txt","../data/gestures/b4.txt",n,m);
    hmm HMM5("../data/gestures/a5.txt","../data/gestures/b5.txt",n,m);


    char str0[] = "gesture 1";
    char str1[] = "gesture 2";
    char str2[] = "gesture 3";
    char str3[] = "gesture 4";
    char str4[] = "gesture 5";

    char *ademan[5];

    ademan[0] = str0;
    ademan[1] = str1;
    ademan[2] = str2;
    ademan[3] = str3;
    ademan[4] = str4;

    while (skeleton) {
        observations obs; //Object that collect the observations
        dynamic_win *windows = new dynamic_win[sf];
        float *votes = new float[M]; //Arary de los votos para cada ademan

        band = false;
        t = 0;
        l = 0;
        k = 0;

        while (t <= sf) {
            l++; //Esto se debe adecuar
            obs.add_observation(x_hand, y_hand, z_hand, x_neck, y_neck); // Esto se debe adecuar
            if ((l % delta) == 0) {	//Determina si el num de frames es mutiplo de delta
                NODO * tmp;
                double max;
                for (i = 0; i < t; i++) { //Por cada ventana existente

                    tmp = obs.get_values(windows[i].get_start_pnt());
                    // Lo siguiente es temporal. Busca el modelo que arroja la
                    //mayor probabilidad
                    max = HMM1.ForwardWithScale(l, tmp);
                    j = 1;
                    if (HMM2.ForwardWithScale(l, tmp) > max) {
                        max = HMM2.ForwardWithScale(l, tmp);
                        j = 2;
                    }
                    else if (HMM3.ForwardWithScale(l, tmp) > max) {
                        max = HMM3.ForwardWithScale(l, tmp);
                        j = 3;
                    }
                    else if (HMM4.ForwardWithScale(l, tmp) > max) {
                        max = HMM4.ForwardWithScale(l, tmp);
                        j = 4;
                    }
                    else if (HMM5.ForwardWithScale(l, tmp) > max) {
                        max = HMM5.ForwardWithScale(l, tmp);
                        j = 5;
                    }

                    windows[i].set_end_pnt(l);
                    windows[i].set_probability(max);

                    votes[j] += max;
                }
                //Se crea la nueva ventana
                tmp = obs.get_values(k);
                // Lo siguiente es temporal. Busca el modelo que arroja la
                //mayor probabilidad
                max = HMM1.ForwardWithScale(l, tmp);
                j = 1;
                if (HMM2.ForwardWithScale(l, tmp) > max) {
                    max = HMM2.ForwardWithScale(l, tmp);
                    j = 2;
                }
                else if (HMM3.ForwardWithScale(l, tmp) > max) {
                    max = HMM3.ForwardWithScale(l, tmp);
                    j = 3;
                }
                else if (HMM4.ForwardWithScale(l, tmp) > max) {
                    max = HMM4.ForwardWithScale(l, tmp);
                    j = 4;
                }
                else if (HMM5.ForwardWithScale(l, tmp) > max) {
                    max = HMM5.ForwardWithScale(l, tmp);
                    j = 5;
                }

                windows[t] = dynamic_win(k,l,max,10);

                votes[j] += max;
                if (votes[j] >= t*threshold) {
                    band = true;
                    recog = j;
                }
                else {
                    if (band) {
                        cout << ademan[recog]; //Esto hay que cambiarlo
                        break;
                    }
                }
                k = l; //Se mueve el punto de inicio para la prox. ventana
                t++; //Incremento del numero de ventanas activas
            }
        }
        // Se reinicia todo


        cout << "Reinicio";  //Esto hay que cambiarlo
    }



}



void Gestures::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}

void Gestures::stop()
{

}



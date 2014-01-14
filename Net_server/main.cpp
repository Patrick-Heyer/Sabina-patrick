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
    sleep(10);


    ifstream infile("../data/Names");
    vector<string> names;
    string line;
    while (getline(infile, line))
    {
        names.push_back(line);
    }

    ifstream infile3("../data/Objects");
    vector<string> objects;
    while (getline(infile3, line))
    {
        objects.push_back(line);
    }

    int name_count=0;
    string accion;
    std::stringstream orden;
    orden << "i will bring ";

    int nothing=0;
    int room=0;
    int tries=0;
    while(1)
    {
        accion= patrol->getInstance().get_Action();

        if(accion=="detectar_incendio")
        {
            net->Read();
            if(net->GetStatus()>0)
            {
                string s = net->GetIncoming();
                cout << s << endl;
                sleep(1);
                net->messages.clear();
                if (s.find("FIRE")!=s.npos)
                {
                    patrol->getInstance().Sintetizer.set_Phrase("There is a fire in the apartment i better go save everybody");
                    sleep(3);
                    patrol->getInstance().set_Action(cambiar_estado("incendio_detectado","si"));
                }
            }

        }

        if(accion=="aprender_orden")
        {
            patrol->getInstance().Sintetizer.set_Phrase("please tell me what object you want");
            sleep(4);
            net->Read();
            if(net->GetStatus()>0)
            {
                std::stringstream ss;
                string s = net->GetIncoming();
                cout << s << endl;
                sleep(2);
                net->messages.clear();
                for (int i=0; i<objects.size(); i++) {
                    print(cons," %s \n",objects[i].c_str());
                    if (s.find((objects[i]))!=s.npos)
                    {
                        patrol->getInstance().Microphone.set_Phrase(objects[i]);
                        sleep(2);
                        ss << "i will take " << objects[i];
                        patrol->getInstance().Sintetizer.set_Phrase(ss.str().c_str());
                        sleep(3);
                        patrol->getInstance().set_Current_destination("DINNING");
                        cambiar_estado("ruta_planeada", "no");
                        cambiar_estado("destino_alcanzado", "no");
                        patrol->getInstance().set_Action(cambiar_estado("orden_pedida", "si"));
                        break;
                    }

                }


            }
            sleep(3);

        }

        if(accion=="aprender_nombre")
        {
            patrol->getInstance().Sintetizer.set_Phrase("Please tell me your name");
            sleep(2);
            net->Read();
            if(net->GetStatus()>0)
            {
                std::stringstream ss;
                string s = net->GetIncoming();
                cout << s << endl;
                sleep(1);


                for (int i=0; i<names.size(); i++) {
                    print(cons," %s \n",names[i].c_str());
                    if (s.find((names[i]))!=s.npos)
                    {
                        patrol->getInstance().Microphone.set_Phrase(names[i].c_str());
                        ss << "i learnd your name " << names[i].c_str();
                        patrol->getInstance().Sintetizer.set_Phrase(ss.str().c_str());
                        sleep(1);
                        ss.str("");
                        orden << names[i].c_str() << " a ";
                        patrol->getInstance().Microphone.set_Words(ss.str().c_str());
                        patrol->getInstance().set_Action(cambiar_estado("aprendido_nombre", "si"));
                        break;
                    }
                }



            }
            sleep(3);

        }


        if(accion=="aprender_mesa")
        {

            net->Read();
            if(net->GetStatus()>0)
            {
                string s = net->GetIncoming();
                cout << s << endl;
                sleep(1);
                if(s!="NULL")
                {
                    patrol->getInstance().Microphone.set_Phrase(s.c_str());

                    patrol->getInstance().set_Action("memorizar_cordenadas");
                }
            }
            sleep(3);
        }

        if(accion=="aprender_drinks")
        {

            net->Read();
            if(net->GetStatus()>0)
            {
                string s = net->GetIncoming();
                cout << s << endl;
                sleep(1);
                if(s!="NULL")
                {
                    patrol->getInstance().Microphone.set_Phrase(s.c_str());

                    patrol->getInstance().set_Action("memorizar_cordenadas");
                }
            }
            sleep(3);
        }
        if(accion=="aprender_snaks")
        {

            net->Read();
            if(net->GetStatus()>0)
            {
                string s = net->GetIncoming();
                cout << s << endl;
                sleep(1);
                if(s!="NULL")
                {
                    patrol->getInstance().Microphone.set_Phrase(s.c_str());

                    patrol->getInstance().set_Action("memorizar_cordenadas");
                }
            }
            sleep(3);
        }


        if(accion=="esperar_confirmacion")
        {
            //GUI::getInstance().Set_Active_Tab("Voice_Recognition");
            patrol->getInstance().Sintetizer.set_Phrase("please tell me exit when you whant me to leave");
            sleep(6);
            net->Read();
            if(net->GetStatus()>0)
            {
                string s = net->GetIncoming();
                cout << s << endl;
                sleep(1);
                cout<<"LEIDO "<<s<<endl;
                if (s.find("EXIT")!=s.npos)
                {
                    patrol->getInstance().Sintetizer.set_Phrase("thank you");
                    patrol->getInstance().set_Current_destination("EXIT");
                    cambiar_estado("ruta_planeada", "no");
                    cambiar_estado("destino_alcanzado", "no");

                    sleep(1);

                    patrol->getInstance().set_Action(cambiar_estado("confirmado","si"));
                }


            }
            sleep(3);
        }


        if(accion=="encontrar_persona")
        {
            bool saved=false;
            if(patrol->getInstance().get_Current_destination()=="EXIT")
            {
                switch ( room )
                {
                case 1:
                    patrol->getInstance().set_Current_destination("ROOM");
                    break;
                case 2:
                    patrol->getInstance().set_Current_destination("LIVING");
                    break;
                case 3:
                    patrol->getInstance().set_Current_destination("DINNING");
                    break;
                default:
                    patrol->getInstance().set_Current_destination("EXIT");
                    break;
                }
                cambiar_estado("ruta_planeada", "no");
                patrol->getInstance().set_Action(cambiar_estado("destino_alcanzado", "no"));
                break;
            }

            //GUI::getInstance().Set_Active_Tab("Voice_Recognition");
            patrol->getInstance().Sintetizer.set_Phrase("is someone here that needs help?");
            sleep(3);

            net->Read();
            if(net->GetStatus()>0)
            {
                string s = net->GetIncoming();
                cout << s << endl;
                sleep(1);
                if (s.find("YES")!=s.npos)
                {
                    do
                    {
                        patrol->getInstance().Sintetizer.set_Phrase("Can you move");
                        sleep(3);
                        s="";
                        net->messages.clear();
                        net->Read();
                        if(net->GetStatus()>0)
                        {
                            s = net->GetIncoming();
                        }
                        net->Read();
                        if(s.find("YES")!=s.npos)
                        {
                            do
                            {
                                patrol->getInstance().Sintetizer.set_Phrase("Can you get out on your own");
                                sleep(3);
                                s="";
                                net->Read();
                                if(net->GetStatus()>0)
                                {
                                    s = net->GetIncoming();
                                    cout << s << endl;
                                    if (s.find("YES")!=s.npos)
                                    {
                                        saved=true;
                                        patrol->getInstance().Sintetizer.set_Phrase("I will go help someone else");
                                        //marcar en mapa supuestamentesalio
                                        room++;
                                        switch ( room )
                                        {
                                        case 1:
                                            patrol->getInstance().set_Current_destination("ROOM");
                                            break;
                                        case 2:
                                            patrol->getInstance().set_Current_destination("LIVING");
                                            break;
                                        case 3:
                                            patrol->getInstance().set_Current_destination("DINNING");
                                            break;
                                        default:
                                            patrol->getInstance().set_Current_destination("EXIT");
                                            break;

                                        }


                                        cambiar_estado("ruta_planeada", "no");
                                        patrol->getInstance().set_Action(cambiar_estado("destino_alcanzado", "no"));
                                        sleep(3);
                                    }
                                    if (s.find("NO")!=s.npos)
                                    {
                                        saved=true;
                                        patrol->getInstance().Sintetizer.set_Phrase("please follow me to the exit");
                                        //marcar en mapa como salvado
                                        room++;
                                        patrol->getInstance().set_Current_destination("EXIT");
                                        cambiar_estado("ruta_planeada", "no");
                                        patrol->getInstance().set_Action(cambiar_estado("destino_alcanzado", "no"));
                                        sleep(3);
                                        s="";
                                    }
                                }
                            } while(!saved);

                        }
                        if(s.find("NO")!=s.npos)
                        {
                            //marcar en mapa herido
                            saved=true;
                            patrol->getInstance().Sintetizer.set_Phrase("I will send help as soon as i can");
                            //marcar en mapa supuestamentesalio
                            room++;
                            switch ( room )
                            {
                            case 1:
                                patrol->getInstance().set_Current_destination("ROOM");
                                break;
                            case 2:
                                patrol->getInstance().set_Current_destination("LIVING");
                                break;
                            case 3:
                                patrol->getInstance().set_Current_destination("DINNING");
                                break;
                            default:
                                patrol->getInstance().set_Current_destination("EXIT");
                                break;

                            }
                        }

                    } while(!saved);


                }
                else
                {
                    sleep(3);
                    tries++;
                    if (tries >=5);
                    {
                        room++;
                        switch ( room )
                        {
                        case 1:
                            patrol->getInstance().set_Current_destination("ROOM");
                            break;
                        case 2:
                            patrol->getInstance().set_Current_destination("LIVING");
                            break;
                        case 3:
                            patrol->getInstance().set_Current_destination("DINNING");
                            break;
                        default:
                            patrol->getInstance().set_Current_destination("EXIT");
                            break;

                        }
                        cambiar_estado("ruta_planeada", "no");
                        patrol->getInstance().set_Action(cambiar_estado("destino_alcanzado", "no"));
                        tries=0;
                    }
                }

            }

            else
            {
                sleep(3);
                cout <<"Nothing" << endl;
                nothing++;
                if (nothing >=5);
                {
                    room++;
                    switch ( room )
                    {
                    case 1:
                        patrol->getInstance().set_Current_destination("ROOM");
                        break;
                    case 2:
                        patrol->getInstance().set_Current_destination("LIVING");
                        break;
                    case 3:
                        patrol->getInstance().set_Current_destination("DINNING");
                        break;
                    default:
                        patrol->getInstance().set_Current_destination("EXIT");
                        break;

                    }
                    cambiar_estado("ruta_planeada", "no");
                    patrol->getInstance().set_Action(cambiar_estado("destino_alcanzado", "no"));
                    nothing=0;
                }
            }
            sleep(3);

        }



    }
    usleep(100);
}



void Net_server::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}

void Net_server::stop()
{

}



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
#include <fstream>
#include <string>
#include <vector>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>

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
#include "../Gui/button.h"

#include "../MDP/MDP.h"

Robot *patrol;

console *cons;
tab *current_tab;
Input_Singleton *teclado;

#include "tty-continuous.h"

#define WIDTH 1440
#define HEIGHT 900

button *obje_butt[10];
button *name_butt[10];
button *dest_butt[10];

#define PLUGIN_NAME "Voice_Recognition"

class Voice_Recognition : public IPlugin
{
public:
    void Main();
    void run();
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Voice_Recognition;
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

void Voice_Recognition::Main()
{
    GUI::GetInstance();
    current_tab = new tab("Voice_Recognition");
    cons= new console(WIDTH/2,HEIGHT*.02,WIDTH/2,HEIGHT/2, "Error", current_tab);
    int num_arg;
    num_arg=25;
    char *arg[25];
                                                                                                                                            
    arg[0]="./arg_test";
    arg[1]="-live";
    arg[2]="TRUE";
    arg[3]="-bestpath";
    arg[4]="TRUE";
    arg[5]="-kbdumpdir";
    arg[6]="/usr/share/sphinx2";
    arg[7]="-lmfn";
    arg[8]="../data/Voice/7132.lm";
    arg[9]="-dictfn";
    arg[10]="../data/Voice/7132.dic";
    arg[11]="-phnfn";
    arg[12]="/usr/share/sphinx2/model/hmm/6k/phone";
    arg[13]="-mapfn";
    arg[14]="/usr/share/sphinx2/model/hmm/6k/map";
    arg[15]="-hmmdir";
    arg[16]="/usr/share/sphinx2/model/hmm/6k";
    arg[17]="-hmmdirlist";
    arg[18]="/usr/share/sphinx2/model/hmm/6k";
    arg[19]="-8bsen";
    arg[20]="TRUE";
    arg[21]="-sendumpfn";
    arg[22]="/usr/share/sphinx2/model/hmm/6k/sendump";
    arg[23]="-cbdir";
    arg[24]="/usr/share/sphinx2/model/hmm/6k";
    
    
    
   sphinx2_init(num_arg,arg);
   ifstream infile("../data/Names");
   vector<string> names;
   string line;
   int nam_butt=0;
   while (getline(infile, line))
   {
       names.push_back(line);
       name_butt[nam_butt]=new button(10+(160*nam_butt),HEIGHT-100,150,24,line.c_str(),current_tab,false);
       nam_butt++;
   }
   
   infile.close();
   
   ifstream infile2("../data/Destinations");
   vector<string> places;
   int des_butt=0;
   while (getline(infile2, line))
   {
       places.push_back(line);
       dest_butt[des_butt]=new button(10+(160*des_butt),HEIGHT-100,150,24,line.c_str(),current_tab,false);
       des_butt++;
   }
   
   infile.close();
   
   ifstream infile3("../data/Objects");
   vector<string> objects;
   int obj_butt=0;
   while (getline(infile3, line))
   {
       objects.push_back(line);
       obje_butt[obj_butt]=new button(10+(160*obj_butt),HEIGHT-100,150,24,line.c_str(),current_tab,false);
       obj_butt++;
   }
   
   infile3.close();
   
   std::string accion;
   for (;;)
   { 
      char *sphinx_result;
//    sphinx_result=" ";
//    sphinx_result=sphinx2_listen(cons);
//    print(cons,"%s \n",sphinx_result);
//    patrol->get_Instance().Microphone.set_Phrase(sphinx_result);
//    
//    std::string temp=sphinx_result;
// 
//    if (temp.find("AMY")!=temp.npos )
//    {
//        print(cons,"Correct \n");
//    }
//    
       accion=patrol->get_Instance().get_Action();
       if(accion=="esperar_confirmacion")
       {
	   GUI::GetInstance().Set_Active_Tab("Voice_Recognition");
	   patrol->get_Instance().Sintetizer.set_Phrase("please tel me when to go");
	   sleep(1);
	   
	   sphinx_result=" ";
	   sphinx_result=sphinx2_listen(cons);
	   print(cons,"%s \n",sphinx_result);
	   patrol->get_Instance().Microphone.set_Phrase(sphinx_result);
	   	
	std::string temp=sphinx_result;
	if (temp.find("GO")!= temp.npos)
	{
	    patrol->get_Instance().Sintetizer.set_Phrase("thank you");
	    sleep(1);
	    patrol->get_Instance().set_Action(cambiar_estado("confirmado","si"));
	    
	}
	   
    }
       
       if(accion=="pedir_nombre")
       {
	   GUI::GetInstance().Set_Active_Tab("Voice_Recognition");
	   bool nombre_conocido=false;
	   int tries=0;
	   do
	   {	
	       if(tries<2)
	       {
		   patrol->get_Instance().Sintetizer.set_Phrase("please tell me your name");
		   sphinx_result=" ";
		   sphinx_result=sphinx2_listen(cons);
		   print(cons,"%s \n",sphinx_result);
		   patrol->get_Instance().Microphone.set_Phrase(sphinx_result);
		   sleep(1);
		   
		   for (int i=0; i<names.size(); i++) {
		       print(cons," %s \n",names[i].c_str());
		       if (patrol->get_Instance().Microphone.get_Phrase().find(names[i])!=patrol->get_Instance().Microphone.get_Phrase().npos)
		       {
			   nombre_conocido=true;
			   patrol->get_Instance().Microphone.set_Phrase(names[i]);
		       }
		   }
		   tries++;
	       }
	       else
	       {
		   patrol->get_Instance().Sintetizer.set_Phrase("i cant hear you plase click on your name on the screen");
		   sleep(2);
		   for(int i=0; i<names.size(); i++)
		   {
		       name_butt[i]->SetActive(true);
		       if(name_butt[i]->state)
		       {
			   nombre_conocido=true;
			   patrol->get_Instance().Microphone.set_Phrase(name_butt[i]->GetName().c_str()); 
			   print(cons,"%s\n",name_butt[i]->GetName().c_str());
			   
		       }
		   }
	       }
	       
	   }while (!nombre_conocido);   
	   for(int j=0; j< names.size();j++)
	   {
	       name_butt[j]->state=false;
	       name_butt[j]->SetActive(false);
	   }
	   patrol->get_Instance().Sintetizer.set_Phrase("please look at my camera so i can learn your face");
	   sleep(5);
	   patrol->get_Instance().set_Action(cambiar_estado("nombre_pedido","si"));
       }
       
       
       if(accion=="pedir_destino")
       {
	   GUI::GetInstance().Set_Active_Tab("Voice_Recognition");
	   bool destino_conocido=false;
	   int tries=0;
	   
	   do
	   {	
	       if(tries<2)
	       {
		   patrol->get_Instance().Sintetizer.set_Phrase("please tell me where to go");

		   sphinx_result=" ";
		   sphinx_result=sphinx2_listen(cons);
		   print(cons,"%s \n",sphinx_result);
		   patrol->get_Instance().Microphone.set_Phrase(sphinx_result);
		   sleep(1);
		   
		   for (int i=0; i<places.size(); i++) {
		       print(cons," %s \n",places[i].c_str());
		       if (patrol->get_Instance().Microphone.get_Phrase().find(places[i])!=patrol->get_Instance().Microphone.get_Phrase().npos)
		       {
			   destino_conocido=true;
			   patrol->get_Instance().set_Current_destination (places[i]);
			   patrol->get_Instance().set_prev_destination(places[i]);
		       }
		   }
		   tries++;
	       }
	       else
	       {
		   patrol->get_Instance().Sintetizer.set_Phrase("i cant hear you plase click on my destinations name on the screen");
		   sleep(2);
		   for(int i=0; i<places.size(); i++)
		   {
		       dest_butt[i]->SetActive(true);
		       
		       if(dest_butt[i]->state)
		       {
			   if(dest_butt[i]->state)
			   {
			       destino_conocido=true;
			       patrol->get_Instance().set_Current_destination(dest_butt[i]->GetName().c_str()); 
			       patrol->get_Instance().set_prev_destination(dest_butt[i]->GetName().c_str());
			       print(cons,"%s\n",dest_butt[i]->GetName().c_str());
			       
			   }
			   
		       }
		   }
	       }
	       
	   }while (!destino_conocido);
	   
	   for(int j=0; j< places.size();j++)
	   {
	       dest_butt[j]->state=false;
	       dest_butt[j]->SetActive(false);
	   }

	     
	   
	   std::stringstream ss;
	   ss << "Ok i am going to " << patrol->get_Instance().get_Current_destination().c_str();
	   patrol->get_Instance().Sintetizer.set_Phrase(ss.str());
	   sleep(1);
	   patrol->get_Instance().set_Action(cambiar_estado("tiene_destino","si"));
       }
       
       
       
       if(accion=="solicitar_objeto")
       {
	   GUI::GetInstance().Set_Active_Tab("Voice_Recognition");
	   bool destino_conocido=false;
	   int tries=0;
	   
	   do
	   {	
	       if(tries<2)
	       {
		   patrol->get_Instance().Sintetizer.set_Phrase("please tell me what you want to drink");

		   sphinx_result=" ";
		   sphinx_result=sphinx2_listen(cons);
		   print(cons,"%s \n",sphinx_result);
		   patrol->get_Instance().Microphone.set_Phrase(sphinx_result);
		   sleep(1);
		   
		   for (int i=0; i<objects.size(); i++) {
		       print(cons," %s \n",objects[i].c_str());
		       if (patrol->get_Instance().Microphone.get_Phrase().find(objects[i])!=patrol->get_Instance().Microphone.get_Phrase().npos)
		       {
			   destino_conocido=true;
			   patrol->get_Instance().set_Current_destination (objects[i]);
		       }
		   }
		   tries++;
	       }
	       else
	       {
		   patrol->get_Instance().Sintetizer.set_Phrase("i cant hear you plase click on your drink on the screen");
		   sleep(2);
		   for(int i=0; i<objects.size(); i++)
		   {
		       obje_butt[i]->SetActive(true);
		       
		       if(obje_butt[i]->state)
		       {
			   if(obje_butt[i]->state)
			   {
			       destino_conocido=true;
			       *patrol->get_Instance().Last_objective= obje_butt[i]->GetName().c_str();
			       
			   }
			   
		       }
		   }
	       }
	       
	   }while (!destino_conocido);
	   
	   for(int j=0; j< objects.size();j++)
	   {
	       obje_butt[j]->state=false;
	       obje_butt[j]->SetActive(false);
	   }
	   
	   
	   
	   std::stringstream ss;

	   patrol->get_Instance().Sintetizer.set_Phrase("Ok i am going to the kitchen for it ");
	   sleep(2);
	   
	   patrol->get_Instance().set_Current_destination("KITCHEN");
	   cambiar_estado("ruta_planeada", "no");
	   cambiar_estado("destino_alcanzado","no");
	   patrol->get_Instance().set_Action(cambiar_estado("objeto_solicitado","si"));
	   
	   
	   
	   
       }
        
    }
    
}

void Voice_Recognition::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}



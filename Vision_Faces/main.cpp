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
		if (accion=="detectar_paciente")
		{
			//GUI::getInstance().Set_Active_Tab("Vision_Faces");
			if (detectFace(frame,&polo)!=NULL)
			{
				patrol->getInstance().Sintetizer.set_Phrase("hello DAVID i will take you to your therapy");
		sleep(5);
		patrol->getInstance().set_Action(cambiar_estado("detectada_p","si"));
			}
		}
		
		if (accion=="aprender_persona")
		{
			//GUI::getInstance().Set_Active_Tab("Vision_Faces");
			int aprendi;
			patrol->getInstance().Sintetizer.set_Phrase("Hello i am sabina I am waiting for a user please aproach me ");
			sleep(2);
			do
			{	
				
				
				frame=patrol->getInstance().Ptz.get_image();
				aprendi=learn(frame);
				videoDisplay->SetImage(frame);
			}while(aprendi==0);
			patrol->getInstance().Sintetizer.set_Phrase( "Ok i learnd your face ");
			sleep(3);
			patrol->getInstance().Sintetizer.set_Phrase( " ");
			sleep(7);
			patrol->getInstance().set_Action(cambiar_estado("aprendida_p","si"));
			
			
		}
		
		if (accion=="reconocer_persona")
		{
			//GUI::getInstance().Set_Active_Tab("Vision_Faces");
			int reconocio;
			do
			{
				reconocio=recognize(frame);
				frame=patrol->getInstance().Ptz.get_image();
				
			}while(reconocio==0);


				std::stringstream ss;
				
				ss << "hello here is the " << patrol->getInstance().Microphone.get_Phrase() << " you orderd";
				patrol->getInstance().Sintetizer.set_Phrase(ss.str());
				sleep(3);
				
				patrol->getInstance().set_Action(cambiar_estado("persona_reconocida","si"));

		}
		videoDisplay->SetImage(frame);
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



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

#include "NiHandViewer.h"


//---------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------
#define SAMPLE_XML_PATH "../data/SamplesConfig.xml"

Robot *patrol;

Console *cons;
Tab *pluginTab;

xn::Context		g_context;
xn::ScriptNode	g_scriptNode;


#define PLUGIN_NAME "Gestos"


/**
 * @brief Gestos plugin
 * 
 * This plugin is used to recognize hand gestures using OpenNi with a Kinect sensor
 * @author     Patrick Heyer, patrickhey@prodigy.net.mx, Angel Oca&ntilde;a ahop_289@hotmail.com
 * @date       jul 04, 2012
 * @version    1.0
 **/
class Gestos : public IPlugin
{
public:
    void Main();
	void stop();
    void run();
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Gestos;
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

std::string accion;

void Gestos::Main()
{
    Gui::getInstance();
    pluginTab = new Tab("Gestos");
    
    
    int num_arg;
    num_arg=1;
    char *arg[1];

    arg[0]="./arg_test";
    
    XnStatus				rc;
	xn::EnumerationErrors	errors;

	// Create a context with default settings
	rc = g_context.InitFromXmlFile(SAMPLE_XML_PATH, g_scriptNode, &errors);
	if (rc == XN_STATUS_NO_NODE_PRESENT)
	{
		XnChar strError[1024];
		errors.ToString(strError, 1024);
		printf("%s\n", strError);
// 		return (rc);
	}
	else if (rc != XN_STATUS_OK)
	{
		printf("Open failed: %s\n", xnGetStatusString(rc));
// 		return (rc);
	}

	SimpleViewer& viewer = HandViewer::CreateInstance(g_context);

	rc = viewer.Init(num_arg, arg);
	if (rc != XN_STATUS_OK)
	{
		printf("Viewer init failed: %s\n", xnGetStatusString(rc));
// 		return 1;
	}

	rc = viewer.Run();
	if (rc != XN_STATUS_OK)
	{
		printf("Viewer run failed: %s\n", xnGetStatusString(rc));
// 		return 1;
	}

// 	return 0;
    
}



void Gestos::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}


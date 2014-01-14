

#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <iostream>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Gui/input_singleton.h"
#include "Gui/gui.h"
#include "Gui/tab.h"
#include "Gui/console.h"
#include "Gui/video.h"
#include "Gui/button.h"

#include "Plugin_API/coreapi.h"
#include "Plugin_API/pluginmanager.h"
#include "Shared_memory/Robot.h"

#include "MDP/MDP.h"
#include "Face/Face.h"

#include "ConfigFile.h"
#include "Triplet.h"

Robot *patrol;

Tab *Main_Tab;

IPlugin *Navegacion;
IPlugin *Navigation_Planing;
IPlugin *Vision_Faces;
IPlugin *Vision_Objects;
IPlugin *VisionSurf;
IPlugin *voice_synthesis;
IPlugin *voice;
IPlugin *manipulacion;
IPlugin *Prueba_Dummy;
IPlugin *Server;
IPlugin *Client;
IPlugin *Gestos;

Button *startButton;


char *MDP_path;
bool Load_Navegacion=0;
bool Load_Navigation_Planing=0;
bool Load_Vision_Faces=0;
bool Load_Vision_Objects=0;
bool Load_VisionSurf=0;
bool Load_voice_synthesis=0;
bool Load_voice=0;
bool Load_manipulacion=0;
bool Load_dummy=0;
bool Load_server=0;
bool Load_client=0;
bool Load_gestos=0;

void Load_config(string filename)
{
	ConfigFile config( filename );
	
	int temp_int;
	float temp_float;
	string temp_string;
	config.readInto(temp_string,"MDP");
	
	MDP_path = new char[temp_string.length() + 1];
	strcpy(MDP_path, temp_string.c_str());
	config.readInto(Load_Navegacion,"Load_Navegacion");
	config.readInto(Load_Navigation_Planing , "Load_Navigation_Planing" );
	config.readInto(Load_Vision_Faces , "Load_Vision_Faces" );
	config.readInto(Load_Vision_Objects , "Load_Vision_Objects" );
	config.readInto(Load_VisionSurf , "Load_VisionSurf" );
	config.readInto(Load_voice_synthesis , "Load_voice_synthesis" );
	config.readInto(Load_voice , "Load_voice" );
	config.readInto(Load_manipulacion , "Load_manipulacion" );
	config.readInto(Load_dummy , "Load_dummy" );
	config.readInto(Load_server, "Load_server");
	config.readInto(Load_client, "Load_client");
	config.readInto(Load_gestos, "Load_gestos");
	
	
}

void InitGL ( GLsizei Width, GLsizei Height )
{
	
	glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glClearColor ( 0.7f, 0.7f, 0.7f, 1.0f );
	glColor4f ( 1.0f, 1.0f, 1.0f, 1.0f );
	glColor3f ( 1.0f, 1.0f, 1.0f );
	glPointSize ( 50 );
	glLineWidth ( 2 );
	glEnable ( GL_CULL_FACE ); //disable this befor drawing gui
	glEnable ( GL_BLEND );
	glEnable ( GL_TEXTURE_2D );
	glEnable ( GL_DEPTH_TEST );
	glBlendFunc ( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );
	glBlendEquation ( GL_FUNC_ADD );
	glDepthFunc ( GL_LEQUAL );
	glEnable ( GL_ALPHA_TEST );
	glAlphaFunc ( GL_GREATER, 0.001f );
	glPolygonMode ( GL_FRONT, GL_FILL );
	
	
	Gui::getInstance().initialize();
	patrol->Initialize("../data/conf");
	initFace();
	
	if(Load_Navegacion)
	{
		PluginManager::getInstance().Load("Navegacion/libnavegacion.so");
	Navegacion = PluginFactory::CreatePlugin("Navegacion");
	if (! Navegacion)
	{
		std::cout << "Failed to load 'Navegacion' plugin!" << std::endl;
		
	}
	}
	
	if(Load_Navigation_Planing)
	{
		PluginManager::getInstance().Load("Navegation_Planing/libnavigation_planing.so");
		Navigation_Planing = PluginFactory::CreatePlugin("Navigation_Planing");
		if (! Navigation_Planing)
		{
			std::cout << "Failed to load 'Navigation_Planing' plugin!" << std::endl;
			
		}
	}
	
	if(Load_Vision_Faces)
	{
		PluginManager::getInstance().Load("Vision_Faces/libvision_faces.so");
		Vision_Faces = PluginFactory::CreatePlugin("Vision_Faces");
		if (! Vision_Faces)
		{
			std::cout << "Failed to load 'Vision_Faces' plugin!" << std::endl;
			
		}
	}
	
	if(Load_voice_synthesis)
	{
		PluginManager::getInstance().Load("Voice_Synthesis/libvoice_synthesis.so");
		voice_synthesis = PluginFactory::CreatePlugin("Voice_Synthesis");
		if (! voice_synthesis)
		{
			std::cout << "Failed to load 'VoiceSynthesis' plugin!" << std::endl;
			
		}
	}
	
	if(Load_dummy)
	{
		PluginManager::getInstance().Load("Prueba_Dummy/libPrueba_Dummy.so");
		Prueba_Dummy = PluginFactory::CreatePlugin("Prueba_Dummy");
		if (! Prueba_Dummy)
		{
			std::cout << "Failed to load 'Dummy' plugin!" << std::endl;
			
		}
	}
	
	if(Load_voice)
	{
		PluginManager::getInstance().Load("Voice_Recognition/libvoice_recognition.so");
		voice = PluginFactory::CreatePlugin("Voice_Recognition");
		if (! voice)
		{
			std::cout << "Failed to load 'Voice_Recognition' plugin!" << std::endl;
			
		}
	}
	
	if(Load_manipulacion)
	{
		PluginManager::getInstance().Load("Manipulacion/libmanipulacion.so");
		manipulacion = PluginFactory::CreatePlugin("Manipulacion");
		if (! manipulacion)
		{
			std::cout << "Failed to load 'Manipulacion' plugin!" << std::endl;
			
		}
	}
	
	if(Load_server)
	{
		PluginManager::getInstance().Load("Net_server/libserver.so");
		Server = PluginFactory::CreatePlugin("Net_server");
		if (! Server)
		{
			std::cout << "Failed to load 'Net_server' plugin!" << std::endl;
			
		}
	}
	
	if(Load_client)
	{
		PluginManager::getInstance().Load("Net_client/libclient.so");
		Server = PluginFactory::CreatePlugin("Net_client");
		if (! Server)
		{
			std::cout << "Failed to load 'Net_client' plugin!" << std::endl;
			
		}
	}
	
	if(Load_gestos)
	{
		PluginManager::getInstance().Load("Gestos/libgestos.so");
		Gestos = PluginFactory::CreatePlugin("Gestos");
		if (! Gestos)
		{
			std::cout << "Failed to load 'Gestos' plugin!" << std::endl;
			
		}
	}
	
	if(Load_Vision_Objects)
	{
		PluginManager::getInstance().Load("Vision_Objects/libMINE.so");
		Vision_Objects = PluginFactory::CreatePlugin("Vision_Objects");
		if (! Vision_Objects)
		{
			std::cout << "Failed to load 'Vision_Objects' plugin!" << std::endl;
			
		}
	}
	
	if(Load_VisionSurf)
    {
	PluginManager::getInstance().Load("VisionSurf/libvision_surf.so");
	Vision_Objects = PluginFactory::CreatePlugin("VisionSurf");
	if (! Vision_Objects)
	{
	    std::cout << "Failed to load 'VisionSurf' plugin!" << std::endl;
	    
	}
	}
	
	Main_Tab= new Tab("Main_Tab");
	
	
	startButton=new Button(WIDTH/2 ,HEIGHT/2,150,24,"START",Main_Tab,false);
	startButton->SetActive(true);
	
	PluginManager::getInstance().ExecuteAll();
}



void DrawGLScene ( void )
{
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glLoadIdentity();
	
	Gui::getInstance().Draw();
	if(startButton->state && startButton->Get_Active())
	{
	patrol->getInstance().set_Action(cambiar_estado("iniciado", "si"));
	patrol->getInstance().Main_system=true;
	patrol->getInstance().set_Action(cambiar_estado("localizado", "si"));
	startButton->SetActive(false);
	
	}
	DrawFace();
	
	glutSwapBuffers();
}


void shutdownApp (void)
{
	
}

void idle_func ( void )
{
	Gui::getInstance().proccesInput();
	glFlush();
	glutPostRedisplay ( );
	
	
}

void processMouse ( int button, int state, int x, int y )
{
	InputSingleton::getInstance().ProcessMouse(button, state, x, y, glutGetModifiers());
}


void processMouseActiveMotion ( int x, int y )
{
	InputSingleton::getInstance().ActiveMotion(x,y);
}


void processMousePassiveMotion ( int x, int y )
{
	InputSingleton::getInstance().PassiveMotion(x,y);
}


void processMouseEntry ( int state )
{
	
}


void keyPressed ( unsigned char key, int x, int y )
{
	InputSingleton::getInstance().KeyPressed(key, x, y, glutGetModifiers());
	switch ( key )
	{
		case 27:
			shutdownApp();
			glutDestroyWindow (1);
			exit ( 0 );
			break;
	}
}

void keyReleased(unsigned char key, int x, int y)
{
	InputSingleton::getInstance().KeyReleased(key, x, y);
}

void specialKeyPressed ( int key, int x, int y )
{
	InputSingleton::getInstance().SpecialKeyPressed(key, x, y, glutGetModifiers());
}

void specialKeyReleased ( int key, int x, int y )
{
	InputSingleton::getInstance().SpecialKeyReleased (key, x, y);
}

void reshape ( int w, int h )
{
	if ( h == 0 ) h = 1;
													 
													 glViewport ( 0, 0, ( GLsizei ) w, ( GLsizei ) h );
	
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity ();
	gluPerspective ( 43.0, ( GLfloat ) w/ ( GLfloat ) h, 0.1, 1000.0 );
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ();
	Gui::getInstance().resize(w,h);
}



int main ( int argc, char *argv[] )
{
	Load_config(argv[1]);
	std::cout << "Initializing MDP" << MDP_path<< endl;
	Init_MDP(MDP_path);
	std::cout << "MDP loaded"<< endl;
	glutInit ( &argc, argv );
	glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH );
	glutInitWindowSize ( WIDTH , HEIGHT);
	glutInitWindowPosition ( 0, 0 );
	glutCreateWindow ( "Markovito - INAOE" );
	glutDisplayFunc ( &DrawGLScene );
	//     glutFullScreen();
	glutIdleFunc ( &DrawGLScene );
	glutIdleFunc ( idle_func );
	glutReshapeFunc ( reshape );
	glutKeyboardFunc ( &keyPressed );
	glutKeyboardUpFunc ( &keyReleased);
	glutSpecialFunc ( &specialKeyPressed );
	glutSpecialUpFunc( &specialKeyReleased );
	InitGL ( WIDTH , HEIGHT);
	
	glutMouseFunc ( processMouse );
	glutMotionFunc ( processMouseActiveMotion );
	glutPassiveMotionFunc ( processMousePassiveMotion );
	glutEntryFunc ( processMouseEntry );
	
	glutMainLoop();
	
}

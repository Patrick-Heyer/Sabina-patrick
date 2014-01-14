

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>



#include "Gui/input_singleton.h"
#include "Gui/gui.h"
#include "Gui/tab.h"
#include "Gui/console.h"
#include "Gui/video.h"
#include "Gui/kinect.h"

#include "Plugin_API/coreapi.h"
#include "Plugin_API/pluginmanager.h"
#include "Shared_memory/Robot.h"

Robot *patrol;

#define WIDTH 1440
#define HEIGHT 900

using namespace std;

tab *tab1;
tab *tab2;

kinect *ki1;
kinect *ki2;

IplImage * kinect_depth;
IplImage * kinect_rgb;

console *cons1, *cons2;

IPlugin *Vision;
bool visruning=false;
IPlugin *Vision1;
bool vis1runing=false;


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

    
    GUI::GetInstance().Initialize();
    patrol->Initialize("../data/conf");

    PluginManager::GetInstance().Load("Prueba1/libprueba1.so");
    Vision = PluginFactory::CreatePlugin("Vision");
    if (! Vision)
    {
        std::cout << "Failed to load 'Vision' plugin!" << std::endl;
        
    }

    PluginManager::GetInstance().Load("Prueba2/libprueba2.so");
    Vision1 = PluginFactory::CreatePlugin("Vision1");
    if (! Vision1)
    {
        std::cout << "Failed to load 'Vision1' plugin!" << std::endl;
        
    }
    

    //Vision->run();
    
    tab1= new tab("polo");
    tab2= new tab("polo2");

    
}


void DrawGLScene ( void )
{
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();

    GUI::GetInstance().Draw();
    
    
    glutSwapBuffers();
}


void shutdownApp (void)
{
    
}


void processMouse ( int button, int state, int x, int y )
{
    Input_Singleton::GetInstance().ProcessMouse(button, state, x, y, glutGetModifiers());
}


void processMouseActiveMotion ( int x, int y )
{
    Input_Singleton::GetInstance().ActiveMotion(x,y);
}


void processMousePassiveMotion ( int x, int y )
{
    Input_Singleton::GetInstance().PassiveMotion(x,y);
}


void processMouseEntry ( int state )
{
    
}


void idle_func ( void )
{
    GUI::GetInstance().ProccesInput();
    glFlush();
    glutPostRedisplay ( );
    
    
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
    GUI::GetInstance().Resize(w,h);
}


void keyPressed ( unsigned char key, int x, int y )
{
    switch ( key )
    {
        case 27:
            shutdownApp();
            glutDestroyWindow (1);
            exit ( 0 );
            break;
    }
}


void specialKeyPressed ( int key, int x, int y )
{
    int mod = glutGetModifiers();
    switch ( key )
    {
        case GLUT_KEY_F1:
            GUI::GetInstance().Set_Active_Tab(0);
            break;
        case GLUT_KEY_F2:
            GUI::GetInstance().Set_Active_Tab(1);
            break;
        case GLUT_KEY_F3:
            GUI::GetInstance().Set_Active_Tab(2);
            break;
        case GLUT_KEY_F4:
            GUI::GetInstance().Set_Active_Tab(3);
            break;
        case GLUT_KEY_F5:
            GUI::GetInstance().Set_Active_Tab(4);
            break;
        case GLUT_KEY_F6:
            GUI::GetInstance().Set_Active_Tab(5);
            break;
        case GLUT_KEY_F7:
            GUI::GetInstance().Set_Active_Tab(6);
            break;
        case GLUT_KEY_F8:
            GUI::GetInstance().Set_Active_Tab(7);
            break;
        case GLUT_KEY_F9:
            GUI::GetInstance().Set_Active_Tab(8);
            break;
        case GLUT_KEY_F10:
            GUI::GetInstance().Set_Active_Tab(9);
            break;
        case GLUT_KEY_F11:
            GUI::GetInstance().Set_Active_Tab(10);
            break;
        case GLUT_KEY_F12:
            GUI::GetInstance().Set_Active_Tab(11);
            break;
        case GLUT_KEY_PAGE_UP:
            break;
        case GLUT_KEY_PAGE_DOWN:
            break;
        case GLUT_KEY_UP:
            if (!visruning)
            {
            Vision->run();
            visruning=true;
            }
            break;
        case GLUT_KEY_DOWN:
            if (!vis1runing)
            {
                Vision1->run();
                vis1runing=true;
            }
            break;
        case GLUT_KEY_LEFT:
            
            break;
        case GLUT_KEY_RIGHT:
            
            break;
        default:
            break;
    }
}


int main ( int argc, char *argv[] )
{
    
    glutInit ( &argc, argv );
    glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH );
    glutInitWindowSize ( 1440, 900 );
    glutInitWindowPosition ( 0, 0 );
    glutCreateWindow ( "Markovito - INAOE" );
    glutDisplayFunc ( &DrawGLScene );
    glutFullScreen();
    glutIdleFunc ( &DrawGLScene );
    glutIdleFunc ( idle_func );
    glutReshapeFunc ( reshape );
    glutKeyboardFunc ( &keyPressed );
    glutSpecialFunc ( &specialKeyPressed );
    InitGL ( 1440, 900 );
    
    glutMouseFunc ( processMouse );
    glutMotionFunc ( processMouseActiveMotion );
    glutPassiveMotionFunc ( processMousePassiveMotion );
    glutEntryFunc ( processMouseEntry );
    
    glutMainLoop();
    
}

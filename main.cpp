

#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <iostream>
#include <dirent.h>
#include <vector>
#include <list>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Gui/input_singleton.h"
#include "Gui/gui.h"
#include "Gui/tab.h"
#include "Gui/console.h"
#include "Gui/video.h"
#include "Gui/button.h"
#include "Gui/textbox.h"


#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.h>
#include <FL/Fl_Input.H>
#include <FL/Fl_Round_Button.H>

#include "Plugin_API/coreapi.h"
#include "Plugin_API/pluginmanager.h"
#include "Shared_memory/Robot.h"

#include "MDP/MDP.h"
#include "Face/Face.h"

#include "ConfigFile.h"
#include "Triplet.h"

Robot *patrol;

Tab *Main_Tab;



Button *startButton;

char *mdp_path;
string face_dir;
bool Load_Face;
bool accept_button=0;

int test_time = 0;
int exit_time = 0;

Fl_Window win( 600,500,"Configure Test" );
Fl_Choice *Test;
Fl_Input *MDP_path;
Fl_Input *Test_time;
Fl_Input *Exit_time;
Fl_Round_Button *rbutton[250];

Fl_Round_Button *use_Face;
int num_rbuttons=0;
Fl_Button *Accept;


char* itoa(int val, int base) {

    static char buf[32] = {0};

    int i = 30;

    for(; val && i ; --i, val /= base)

        buf[i] = "0123456789abcdef"[val % base];

    return &buf[i+1];

}

void Run_test(Fl_Widget* o, void*)
{
    for(int j=0; j<num_rbuttons; j++)
    {
        if(!rbutton[j]->value())
        {
            PluginManager::getInstance().Unload(rbutton[j]->label());
        }
    }

    win.hide();
    Fl::flush();
    accept_button=true;
}


void Load_test(Fl_Widget* o, void*)
{
    std::stringstream ss;
    ss << "../data/Tests/" << Test->mvalue()->label();
    std::string test_path = ss.str();
    std::cout << test_path << std::endl;

    ConfigFile config ( test_path );

    PluginManager::getInstance().LoadFromFile ( "../data/TEST_plugins" );

    win.begin();
    int row=0;
    int column=0;
    for( PluginManager::TListItor itor = PluginManager::getInstance().m_pluginlist.begin(); itor != PluginManager::getInstance().m_pluginlist.end(); ++itor )
    {
        rbutton[num_rbuttons] = new Fl_Round_Button(20+(column*170), 120+(30*row), 150, 30, (*itor)->GetDisplayName().c_str());
        num_rbuttons++;
        row++;
        if(row%10==0)
        {
            column++;
            row=0;
        }

    }
    
    int temp_int;
    float temp_float;
    string temp_string;
    config.readInto ( temp_string, "MDP" );
    mdp_path = new char[temp_string.length() + 1];
    strcpy ( mdp_path, temp_string.c_str() );
    MDP_path->value(mdp_path);

    config.readInto ( face_dir, "face_dir" );
    config.readInto ( test_time, "test_time" );
    Test_time->value(itoa (test_time,10));

    config.readInto ( exit_time, "exit_time" );

    Accept= new Fl_Button( 250, 450, 100, 30, "Run Test" );
    Accept->callback(Run_test);

    o->hide();
    win.end();

}



void Load_config ( string filename )
{

    win.begin();


    Test= new Fl_Choice(80,30, 150, 30, "Test to run");
    Test_time = new Fl_Input(480, 30, 100, 30, "Test Time");
    Exit_time = new Fl_Input(480, 60, 100, 30, "Exit Time");
    MDP_path = new Fl_Input(80, 60, 150, 30, "MDP");

    std::ifstream inFile;
    std::string Test_name;



    inFile.open(filename.c_str());
    if (!inFile) {
        std::cout << "Unable to open file" <<std::endl;
    }

    while (inFile >> Test_name) {
        Test->add(Test_name.c_str(),0,NULL);
    }


    Test->value(0);
    Fl_Button load_test( 300, 30, 100, 30, "Load Test" );

    use_Face=new Fl_Round_Button(80,90, 150, 30, "With FACE");
    load_test.callback( Load_test);

    inFile.close();
    win.end();

    win.show();
    Fl::run();
    while(!accept_button)
    {

    }

}

void InitGL ( GLsizei Width, GLsizei Height )
{

    glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR );
    glClearColor ( 0.7f, 0.7f, 0.7f, 1.0f );
    glColor4f ( 1.0f, 1.0f, 1.0f, 1.0f );
    glColor3f ( 1.0f, 1.0f, 1.0f );
    glPointSize ( 50 );
    glLineWidth ( 2 );
    glEnable ( GL_CULL_FACE ); //disable this befor drawing gui
    glEnable ( GL_BLEND );
    glEnable ( GL_TEXTURE_2D );
    glEnable ( GL_DEPTH_TEST );
    glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glBlendEquation ( GL_FUNC_ADD );
    glDepthFunc ( GL_LEQUAL );
    glEnable ( GL_ALPHA_TEST );
    glAlphaFunc ( GL_GREATER, 0.001f );
    glPolygonMode ( GL_FRONT, GL_FILL );


    Gui::getInstance().initialize();
    patrol->Initialize ( "../data/conf" );
    patrol->getInstance().seguir = false;
    if (face_dir!="" && use_Face->value())
    {
        std::cout << "Loading " << face_dir << std::endl;
        initFace ( face_dir );
        setFace ( 0 );
    }
    *patrol->getInstance().seq_path=mdp_path;

    Main_Tab = new Tab ( "Main_Tab" );
    Gui::getInstance().setActiveTab ( 0 );

    PluginManager::getInstance().ExecuteAll();

    startButton = new Button ( WIDTH / 2 , HEIGHT / 2, 150, 24, "START", Main_Tab, false );
    startButton->SetActive ( true );
}

bool polo=true;

void DrawGLScene ( void )
{
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();

    Gui::getInstance().proccesInput();
    Gui::getInstance().Draw();


    if(patrol->getInstance().get_Action()=="seguir" && polo)
    {
        polo=false;



    }

    if ( startButton->state && startButton->Get_Active() )
    {

        patrol->getInstance().set_Action ( "none" );
        patrol->getInstance().Main_system = true;
        startButton->SetActive ( false );

    }

    std::stringstream ss;
    ss << "minutes left: " << patrol->getInstance().test_time / 60;
    if(face_dir!="" && use_Face->value())
    {
        DrawFace();
    }

    glutSwapBuffers();
}

void shutdownApp ( void )
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
    InputSingleton::getInstance().ProcessMouse ( button, state, x, y, glutGetModifiers() );
}

void processMouseActiveMotion ( int x, int y )
{
    InputSingleton::getInstance().ActiveMotion ( x, y );
}

void processMousePassiveMotion ( int x, int y )
{
    InputSingleton::getInstance().PassiveMotion ( x, y );
}

void processMouseEntry ( int state )
{

}

void keyPressed ( unsigned char key, int x, int y )
{
    InputSingleton::getInstance().KeyPressed ( key, x, y, glutGetModifiers() );

    switch ( key )
    {
    case 27:
        shutdownApp();
        glutDestroyWindow ( 1 );
        exit ( 0 );
        break;
    }
}

void keyReleased ( unsigned char key, int x, int y )
{
    InputSingleton::getInstance().KeyReleased ( key, x, y );
}

void specialKeyPressed ( int key, int x, int y )
{
    InputSingleton::getInstance().SpecialKeyPressed ( key, x, y, glutGetModifiers() );
}

void specialKeyReleased ( int key, int x, int y )
{
    InputSingleton::getInstance().SpecialKeyReleased ( key, x, y );
}

void reshape ( int w, int h )
{
    if ( h == 0 )
        h = 1;

    glViewport ( 0, 0, ( GLsizei ) w, ( GLsizei ) h );
    glMatrixMode ( GL_PROJECTION );
    glLoadIdentity ();
    gluPerspective ( 43.0, ( GLfloat ) w / ( GLfloat ) h, 0.1, 1000.0 );
    glMatrixMode ( GL_MODELVIEW );
    glLoadIdentity ();
    Gui::getInstance().resize ( w, h );
}

void Clear_exit()
{
    std::cout << "EXITING" << std::endl;
    PluginManager::getInstance().UnloadAll();
}

int main ( int argc, char *argv[] )
{
    Load_config ( argv[1] );
    std::cout << "Initializing MDP" << mdp_path << endl;
//     Init_MDP ( mdp_path );
    
    std::cout << "MDP loaded" << endl;
    glutInit ( &argc, argv );
    glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH );
    glutInitWindowSize ( WIDTH , HEIGHT );
    glutInitWindowPosition ( 0, 0 );
    glutCreateWindow ( "Markovito - INAOE" );
    glutDisplayFunc ( &DrawGLScene );
//  glutFullScreen();
    glutIdleFunc ( &DrawGLScene );
    glutIdleFunc ( idle_func );
    glutReshapeFunc ( reshape );
    glutKeyboardFunc ( &keyPressed );
    glutKeyboardUpFunc ( &keyReleased );
    glutSpecialFunc ( &specialKeyPressed );
    glutSpecialUpFunc ( &specialKeyReleased );
    InitGL ( WIDTH , HEIGHT );

    glutMouseFunc ( processMouse );
    glutMotionFunc ( processMouseActiveMotion );
    glutPassiveMotionFunc ( processMousePassiveMotion );
    glutEntryFunc ( processMouseEntry );
    atexit ( Clear_exit );
    glutMainLoop();
}

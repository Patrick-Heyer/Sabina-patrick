
#include <iostream>
#include <pthread.h>

#include "Controller.h"

#include "../Plugin_API/pluginapi.h"
#include "../Shared_memory/Robot.h"
#include "../MDP/MDP.h"
#include "../Gui/gui.h"
#include "../Gui/console.h"
#include "../Gui/video.h"


Robot *patrol;
Tab *pluginTab;
//Console *logTerminal;

#define LOCAL_MAP_HEIGHT 500.0
#define LOCAL_MAP_WIDHT 500.0

#define PLUGIN_NAME "Navegacion_Restaurant"

/**
 * @brief Navegacion_Restaurant plugin
 *
 * This plugin is used to perform the movments of the robot based on the route planed by Navigation_Planing
 * @author     Patrick Heyer, patrickhey@prodigy.net.mx Miguel Palacios  mkpalacio@gmail.com
 * @date       jul 04, 2012
 * @version    1.0
 **/


class Navegacion_Restaurant : public IPlugin
{
public:
    void Main();
    void stop();
    void run();
private:
    bool running;
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Navegacion_Restaurant;
}

PLUGIN_FUNC void DestroyPlugin ( IPlugin *r )
{
    delete r;
}

PLUGIN_DISPLAY_NAME ( PLUGIN_NAME );

PLUGIN_INIT()
{
    // register our new plugin
    RegisterPlugin ( PLUGIN_NAME, CreatePlugin, DestroyPlugin );
    return 0;
}

void Navegacion_Restaurant::Main()
{

    std::string accion;
    Location temp;
    CvPoint center;
    std::list<ArPose> path;

    Gui::getInstance();
    pluginTab = new Tab ( "Navegacion_Restaurant" );
//     logTerminal = new Console ( WIDTH / 2, HEIGHT * .02, WIDTH, HEIGHT / 2, "Log_Terminal", pluginTab );


    IplImage *tempMap;
    tempMap = cvLoadImage ( patrol->getInstance().Enviroment.get_Image_path().c_str() );

    patrol->getInstance().Enviroment.set_Height_px ( tempMap->height );
    patrol->getInstance().Enviroment.set_Width_px ( tempMap->width );

    cvReleaseImage ( &tempMap );


    Controller robotController ( ( char * ) patrol->getInstance().get_Aria_port().c_str(), ( char * ) patrol->getInstance().get_Laser_port().c_str() );

    robotController.setAbsoluteMaxTransVelocity ( patrol->getInstance().get_Lineal_velocity() );
    robotController.setAbsoluteMaxRotAcceleration ( patrol->getInstance().get_Angular_velocity() );


    robotController.setOdometer ( patrol->getInstance().Position.get_X() * 50, patrol->getInstance().Position.get_Y() * -50, patrol->getInstance().Position.get_Angle() );
    int rot = 1;

    do
    {
        accion = patrol->getInstance().get_Action();
        temp.set ( robotController.getRobotPositionX() / 50, ( robotController.getRobotPositionY() / 50 ) * -1, robotController.getRobotHeading() );
        patrol->getInstance().setPosition ( temp );

        if ( accion == "seguir" )
        {
            do
                {}
            while ( robotController.isThereObstacle ( 1100 ) );

            patrol->getInstance().Sintetizer.set_Phrase("i will follow you");
            sleep(2);
            robotController.follow();
            std::cout << "deje de seguir" << std::endl;
            patrol->getInstance().set_Action(cambiar_estado("seguimiento_fin", "si"));

        }

        if ( accion == "follow" )
        {
	     do
                {}
            while ( robotController.isThereObstacle ( 1100 ) );

            patrol->getInstance().Sintetizer.set_Phrase("i will follow you");
            sleep(2);
            robotController.follow();

        }

        if ( accion == "entrar" )
        {
//             print ( logTerminal, "Iniciando: entrar\n" );
            printf ( "iniciando entrar" );
//patrol->getInstance().set_Action ( cambiar_estado ( "dentro_arena", "si" ) );
            do
                {}
            while ( robotController.isThereObstacle ( 1100 ) );

            ArPose position ( robotController.getRobotPositionX() + 1500, robotController.getRobotPositionY(), 0 );
            path.clear();
            path.push_back ( position );
            robotController.setPath ( path );
            robotController.moveRobot();
            patrol->getInstance().set_Action ( cambiar_estado ( "dentro_arena", "si" ) );
//             print ( logTerminal, "Terminado: entrar\n" );
        }

        if ( accion == "localizar" )
        {
            patrol->getInstance().set_Action ( cambiar_estado ( "localizado", "si" ) );

        }

        if ( accion == "navegar_destino" )
        {
//             print ( logTerminal, "Iniciando: navegar_destino\n" );
            list<Location> ruta_temp;
            ruta_temp = patrol->getInstance().get_Route();

            if ( ruta_temp.size() > 1 )
            {
                path.clear();

                for ( std::list<Location>::iterator list_iter = ruta_temp.begin(); list_iter != ruta_temp.end(); list_iter++ )
                {
                    ArPose position ( ( *list_iter ).get_X() * 50, ( *list_iter ).get_Y() * -50, ( *list_iter ).get_Angle() );
                    path.push_back ( position );

                }

                robotController.setPath ( path );
                robotController.moveRobot();
            }

            patrol->getInstance().Clear_path();
            robotController.setRobotHeading ( 45 );
            patrol->getInstance().set_Action ( cambiar_estado ( "destino_alcanzado", "si" ) );
//             print ( logTerminal, "Terminado: navegar_destino\n" );
        }
    }
    while ( this->running );
}

void Navegacion_Restaurant::run()
{
    this->running = true;
    pthread_create ( &thread_id, NULL, &IPlugin::IncWrapper, this );
}

void Navegacion_Restaurant::stop()
{

}


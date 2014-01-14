#include "../Plugin_API/pluginapi.h"
#include "../Shared_memory/Robot.h"
#include "../MDP/MDP.h"
#include "../Gui/gui.h"
#include "../Gui/console.h"
#include "../Gui/video.h"

#include "CAmbiente.h"
#include "TiposBasicos.h"
#include "CProbot.h"
#include "Constants.h"
#include "read_prm.h"


Robot *patrol;

Video *videoDisplay;
Tab *pluginTab;
Console *logTerminal;


#define LOCAL_MAP_HEIGHT 500.0
#define LOCAL_MAP_WIDHT 500.0

#define PLUGIN_NAME "Navigation_Planing"

/**
 * @brief Navigation_Planing plugin
 *
 * This plugin is used to plan a safe route for the robot (that will be performed by Navegacion) from its current position to a specific destination.
 * @author     Patrick Heyer, patrickhey@prodigy.net.mx, Miguel Palacios mkpalacio@gmail.com
 * @date       jul 04, 2012
 * @version    1.0
 **/
class Navigation_Planing : public IPlugin
{
public:
    void Main();
    void stop();
    void run();
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Navigation_Planing;
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

void Navigation_Planing::Main()
{
    /**
     * \sa read_prm
     * \sa cambiar_estado
     **/

    Gui::getInstance();
    pluginTab = new Tab("Navigation_Planing");
    logTerminal = new Console(WIDTH/2,HEIGHT*.02,WIDTH,HEIGHT/2,"Log_Terminal", pluginTab);
    videoDisplay = new Video(0,HEIGHT*.02,WIDTH/2,HEIGHT/2,"Navigation_Planing_Display", pluginTab);

    string filename;
    CAmbiente *ambiente_p_v = NULL;
    uu_PlanningParameters parametros_v;
    CProbot *prm;
    prm= new CProbot;
    parametros_v = prm->Parametros();
    ambiente_p_v = (CAmbiente*)prm->Ambiente();

    if (ambiente_p_v != NULL)
    {
        parametros_v.NodeNumber = 1200;
        parametros_v.NeighbourDistance = 16;
        parametros_v.NeighbourNumber = 1200/20;
        ambiente_p_v->setsizeRobot( 17 );
        parametros_v.StepSize = 8;
        parametros_v.SamplingMethod = PRM::GAUSSIAN_S;

        filename = patrol->getInstance().Enviroment.get_Image_path();


        prm->setPlanningParametros(parametros_v);
        print(logTerminal, "Mapa: %s Numero de nodos %d  Distancia entre nodos: %f Tamaño del robot %d \n",
              filename.c_str(), parametros_v.NodeNumber, parametros_v.NeighbourDistance, ambiente_p_v->sizeRobot() );
        if (parametros_v.SamplingMethod == PRM::GAUSSIAN_S )
            print(logTerminal,"Tipo muestreo: Aleatorio Gaussiano\n");
        else
            print(logTerminal,"Tipo muestreo: Aleatorio Uniforme\n");


    }

    CvPoint center;
    list< Location> puntos;

    IplImage *tempMap;
    tempMap=cvLoadImage(patrol->getInstance().Enviroment.get_Image_path().c_str());
    patrol->getInstance().Enviroment.set_Height_px(tempMap->height);
    patrol->getInstance().Enviroment.set_Width_px(tempMap->width);

    IplImage *map= cvCreateImage(cvSize(LOCAL_MAP_HEIGHT,LOCAL_MAP_WIDHT),tempMap->depth, tempMap->nChannels);
    cvResize(tempMap,map);

    std::string accion;
    int initial_x, final_x;
    int initial_y, final_y;

    for (;;)
    {
        accion=patrol->getInstance().get_Action();


        if (accion=="planear_ruta")
        {
            tempMap=cvLoadImage(patrol->getInstance().Enviroment.get_Image_path().c_str());
            patrol->getInstance().Enviroment.set_Height_px(tempMap->height);
            patrol->getInstance().Enviroment.set_Width_px(tempMap->width);
            std::cout << "Loading image " << patrol->getInstance().Enviroment.get_Image_path() << std::endl;
            map= cvCreateImage(cvSize(LOCAL_MAP_HEIGHT,LOCAL_MAP_WIDHT),tempMap->depth, tempMap->nChannels);
            cvResize(tempMap,map);
            filename = patrol->getInstance().Enviroment.get_Image_path();
            ambiente_p_v->setMapaFileName(&filename);
            patrol->getInstance().Sintetizer.set_Phrase("i am planing my route");

// 			Gui::getInstance().setActiveTab("Navigation_Planing");
            //         if (Input_Singleton::getInstance().key=='t')
            typedef std::map<string,Destination> StringFloatMap;
            StringFloatMap::iterator pos;

            initial_x= patrol->getInstance().Position.get_X();
            initial_y= patrol->getInstance().Position.get_Y();

            std::cout << "im planing my route to " << patrol->getInstance().get_Current_destination() << std::endl;
            pos = patrol->getInstance().Destinations->find(patrol->getInstance().get_Current_destination());
            final_x= pos->second.get_Coordinate().get_X();
            final_y= pos->second.get_Coordinate().get_Y();

            center.x=initial_x*(LOCAL_MAP_WIDHT/patrol->getInstance().Enviroment.getWidthPx());
            center.y=initial_y*(LOCAL_MAP_WIDHT/patrol->getInstance().Enviroment.getWidthPx());
            cvCircle(map, center, 5, CV_RGB(0, 0, 255), -1, 8, 0);

            center.x=final_x*(LOCAL_MAP_WIDHT/patrol->getInstance().Enviroment.getWidthPx());
            center.y=final_y*(LOCAL_MAP_WIDHT/patrol->getInstance().Enviroment.getWidthPx());
            cvCircle(map, center, 5, CV_RGB(0, 255, 0), -1, 8, 0);
            videoDisplay->SetImage(map);
            patrol->getInstance().set_Route(read_prm(prm, initial_y, initial_x, final_y, final_x));

            puntos=patrol->getInstance().get_Route();
            if (puntos.size()>1)
            {
                for (std::list<Location>::iterator list_iter = puntos.begin();
                        list_iter != puntos.end(); list_iter++)
                {
                    print(logTerminal,"%d  %d \n ", (*list_iter).get_X(), (*list_iter).get_Y());
                }
                patrol->getInstance().set_Action(cambiar_estado("ruta_planeada","si"));
            }

            videoDisplay->SetImage(map);
        }
        else if (accion=="seguir")
        {
            patrol->getInstance().Enviroment.set_Image_path("../data/map200.png");
            tempMap=cvLoadImage(patrol->getInstance().Enviroment.get_Image_path().c_str());
            patrol->getInstance().Enviroment.set_Height_px(tempMap->height);
            patrol->getInstance().Enviroment.set_Width_px(tempMap->width);

            map= cvCreateImage(cvSize(LOCAL_MAP_HEIGHT,LOCAL_MAP_WIDHT),tempMap->depth, tempMap->nChannels);
            cvResize(tempMap,map);

            do
            {
                accion=patrol->getInstance().get_Action();

                center.x=patrol->getInstance().Position.get_X()*(LOCAL_MAP_WIDHT/patrol->getInstance().Enviroment.getWidthPx());
                center.y=patrol->getInstance().Position.get_Y()*(LOCAL_MAP_HEIGHT/patrol->getInstance().Enviroment.getHeightPx());
                cvCircle(map, center, 20, CV_RGB(255, 255, 255), -1, 8, 0);
                videoDisplay->SetImage(map);
                usleep(500000);
            } while (accion=="seguir");


            IplImage * grayScaleImage = cvCreateImage(cvSize(tempMap->height,tempMap->width),8,1);
            cvResize(map,tempMap);
            cvCvtColor(tempMap, grayScaleImage, CV_BGR2GRAY);


            cvSaveImage("../data/map201.png",grayScaleImage);
            patrol->getInstance().Enviroment.set_Image_path("../data/map201.png");
        }


        else
        {
            puntos=patrol->getInstance().get_Route();
            if (puntos.size()>1)
            {
                center.x=initial_x*(LOCAL_MAP_WIDHT/patrol->getInstance().Enviroment.getWidthPx());
                center.y=initial_y*(LOCAL_MAP_WIDHT/patrol->getInstance().Enviroment.getWidthPx());
                cvCircle(map, center, 4, CV_RGB(0, 0, 255), -1, 8, 0);

                center.x=final_x*(LOCAL_MAP_WIDHT/patrol->getInstance().Enviroment.getWidthPx());
                center.y=final_y*(LOCAL_MAP_WIDHT/patrol->getInstance().Enviroment.getWidthPx());
                cvCircle(map, center, 4, CV_RGB(0, 255, 0), -1, 8, 0);
                for (std::list<Location>::iterator list_iter = puntos.begin();
                        list_iter != puntos.end(); list_iter++)
                {

                    center.x=((*list_iter).get_X())*(LOCAL_MAP_WIDHT/patrol->getInstance().Enviroment.getWidthPx());
                    center.y=((*list_iter).get_Y())*(LOCAL_MAP_WIDHT/patrol->getInstance().Enviroment.getHeightPx());
                    cvCircle(map, center, 2, CV_RGB(255, 0, 0), -1, 8, 0);
                }
            }
            center.x=patrol->getInstance().Position.get_X()*(LOCAL_MAP_WIDHT/patrol->getInstance().Enviroment.getWidthPx());
            center.y=patrol->getInstance().Position.get_Y()*(LOCAL_MAP_HEIGHT/patrol->getInstance().Enviroment.getHeightPx());
            cvCircle(map, center, 3, CV_RGB(0, 0, 255), -1, 8, 0);
            videoDisplay->SetImage(map);
            usleep(500000);
        }
    }
    cvReleaseImage(&map);
    cvReleaseImage(&tempMap);
}

void Navigation_Planing::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}

void Navigation_Planing::stop()
{

}




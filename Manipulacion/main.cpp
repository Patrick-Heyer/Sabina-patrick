#include <iostream>
#include <pthread.h>
#include <vector>

#include "kintoarmtransf.h"

#include "../Gui/gui.h"
#include "../Gui/tab.h"
#include "../Gui/console.h"
#include "../Plugin_API/pluginapi.h"
#include "../Shared_memory/Robot.h"

#include "armkatana.h"
#include "armkatanaforsabina.h"

#include "../MDP/MDP.h"


Robot *patrol;
Tab *pluginTab;

#define PLUGIN_NAME "Manipulacion"

//parametros calculados el dia 9 de diciembre de 2013
//ANGLE- angulo medidoS
#define ANGLE 90

/**
 * @brief Manipulacion plugin
 *
 * This plugin is used for object manipulation using the Katana 6M arm.
 * @author     Patrick Heyer, patrickhey@prodigy.net.mx, Irving V&aacute;squez ivasquez@inaoep.mx
 * @date       jul 04, 2012
 * @version    1.0
 **/
class Manipulacion : public IPlugin
{
public:
    void Main();
    void stop();
    void run();
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Manipulacion;
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

void Manipulacion::Main()
{
    /**
     * \sa cambiar_estado
     **/
    Gui::getInstance();
    pluginTab = new Tab("Manipulacion");

    ArmKatanaForSabina brazo;
    brazo.init( "192.168.168.232", "../data/Arm/katana6M180_G.cfg");
    brazo.calibrate();
    sleep(5);
    brazo.setVelocity(20);
    brazo.moveToHanging();
    brazo.moveToCarriyingPos();

    std::string accion;
    for (;;)
    {
        accion=patrol->getInstance().get_Action();
        if(accion=="sujetar_objeto")
        {

            KinToArmTransf T;

            T.setKinectPosition(-.485,0,-.180,(ANGLE-44)*3.1416/180);

            float x,y,z;
            T.tranformToArm(patrol->getInstance().object_real_X,patrol->getInstance().object_real_Y, patrol->getInstance().object_real_Z, x,y,z);
            brazo.moveToCarriyingPos();
            std::cout << "intentare agarrar el objeto en " <<  x << "    " << y << "    " << z << "MILIMETROS" << std::endl;

	    brazo.Grasping(x, y, z);
            patrol->getInstance().set_Action(cambiar_estado("objeto_sujeto","si"));

        }

        if(accion=="entregar_objeto")
        {
//             patrol->getInstance().Sintetizer.set_Phrase("here is a little present so you remember your visit");
//             sleep(5);
//             brazo.deliverObject();
//             patrol->getInstance().Sintetizer.set_Phrase("please let me introduce myself i am Sabina a service robot developed at the INAOE by the team Markovito that won the mexican tournament of robotics two thousand 13");
//             brazo.moveToHanging();
//
//             //Cuando se tengan que entregar varios objetos no usar store
//             //brazo.store();
            patrol->getInstance().set_Action(cambiar_estado("objeto_entregado","si"));
        }
    }
}

void Manipulacion::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}

void Manipulacion::stop()
{

}



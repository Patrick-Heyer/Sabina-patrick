#include <iostream>
#include <pthread.h>
#include <vector>

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
Console *logTerminal;

#define PLUGIN_NAME "Manipulacion"

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
    logTerminal = new Console(0,HEIGHT*.02,500,HEIGHT/2,"error", pluginTab);
    ArmKatanaForSabina brazo;
    brazo.init( "192.168.168.232", "../data/configfiles450/katana6M180_G.cfg");
    sleep(5);
    brazo.setVelocity(20);
    brazo.moveToHanging();
    brazo.moveToCarriyingPos();

    std::string accion;
    for (;;)
    {
        if(accion=="sujetar_objeto")
        {
            Gui::getInstance().setActiveTab("Manipulacion");
// 			patrol->getInstance().Sintetizer.set_Phrase("please put the object in front of me in my gripper");
            brazo.graspObjectAtKinectImage(0,0);
            patrol->getInstance().set_Action(cambiar_estado("objeto_sujetado","si"));

        }

        if(accion=="entregar_gripper")
        {
            patrol->getInstance().Sintetizer.set_Phrase("Please take the rehabilitation device");
            brazo.deliverObject();
            brazo.moveToHanging();
            brazo.store();
            patrol->getInstance().set_Action(cambiar_estado("entregado_gripper","si"));

        }

        accion=patrol->getInstance().get_Action();
        if(accion=="entregar_objeto")
        {
            Gui::getInstance().setActiveTab("Manipulacion");
            patrol->getInstance().Sintetizer.set_Phrase("Please take the objet of my gripper    Hello good afternoon i am sabina and I am a service robot designed to help you performing tedious tasks at home like serving, cleaning, helping the old and sick. I was built and progrramed  in puebla mexico by the team Markovito from the INAOE directed by Doctor Enrique sucar, Doctor eduardo morales i hope you enjoy my performance");
            brazo.deliverObject();
            brazo.moveToHanging();
            brazo.store();
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



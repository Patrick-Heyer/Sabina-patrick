/**
 * @file
 * This file is the entery point to comunicate with the Katana 6M 450 arm
 *
 * The standard for Sabina plugins is used to start and run this plugin, 
 * its main functionality is implemented in the armkatana and armkatanaforsabina files
 *  
 */

#include <iostream>
#include <pthread.h>

#include <Aria.h>
#include <ArRobot.h>

ArRobot robot;

ArRobotConnector *connector;

#include "../Plugin_API/pluginapi.h"
#include "../Shared_memory/Robot.h"
#include "../Human/Human.h"
#include "../Human/joint.h"
#include "../MDP/MDP.h"
#include "../Gui/gui.h"
#include "../Gui/console.h"
#include "../Gui/video.h"


#include <iostream>
#include <vector>

//#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>


Robot *patrol;
Tab *pluginTab;
Console *logTerminal;

#define LOCAL_MAP_HEIGHT 500.0
#define LOCAL_MAP_WIDHT 500.0


#define PLUGIN_NAME "Follower"

/**
 * @brief Follower plugin
 * 
 * This plugin is used to perform the movments of the robot based on the route planed by Navigation_Planing
 * @author     Patrick Heyer, patrickhey@prodigy.net.mx Miguel Palacios  mkpalacio@gmail.com
 * @date       jul 04, 2012
 * @version    1.0
 **/


class Follower : public IPlugin
{
public:
	void Main();
	void stop();
	void run();
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
	return new Follower;
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


struct mouse_info_struct { int x,y; };
struct mouse_info_struct mouse_info = {-1,-1}, last_mouse;

std::vector<cv::Point> mousev,kalmanv;

void on_mouse(int x, int y, int flags, void* param) {
	//if (event == CV_EVENT_LBUTTONUP) 
	{
		last_mouse = mouse_info;
		mouse_info.x = x;
		mouse_info.y = y;
		
		//		cout << "got mouse " << x <<","<< y <<endl;
	}
}

Video *videoDisplay;
void Follower::Main()
{ 
	  Gui::getInstance();
    pluginTab = new Tab("Follower");
    logTerminal = new Console(WIDTH/2,HEIGHT*.02,WIDTH,HEIGHT/2,"Log_Terminal", pluginTab);
    sleep(10);
    
    Aria::init();
    
    int num_arg;
    num_arg=3;
    char *arg[5];
    
    arg[0]="./arg_test";
    arg[1]="-robotPort";
    arg[2]= "/dev/ttyUSB0";
    
    
    ArArgumentParser parser(&num_arg, arg);
    connector= new ArRobotConnector( &parser, &robot);
    
    parser.loadDefaultArguments();
    
    
    connector->parseArgs();
    
    //robot=new ArRobot();
    
    connector->connectRobot();
    robot.disableSonar();
    robot.runAsync(true);
    
    
    robot.lock(); //Lock robot during set up 18
    robot.comInt(ArCommands::ENABLE, 1); //Turn on the motors 19
    
    robot.unlock(); //Unlock the robot 20
    
//     robot.setAbsoluteMaxTransVel(1000);
//     robot.setAbsoluteMaxRotVel(20);
    
   std::map<std::string, skeletonJoint>::iterator iter;
    double angle;
    int speed=0, rot=0;
    bool obstaculolaser=false;
    skeletonJoint R_arm, L_arm, Torso;
	robot.enableMotors();
	robot.disableMotors();
	robot.enableMotors();
	int veces_r=0;
	int veces_l=0;
	for(;;)
	{
	
        if (Human::getInstance().tracking==false)
        {
            robot.setVel(0);
            robot.setDeltaHeading(0);
            continue;   
        }
        
        for(iter=Human::getInstance().Skeleton->begin(); iter !=Human::getInstance().Skeleton->end(); ++iter)
        {
            if(iter->second.Get_ID()==3) Torso=iter->second;
            if(iter->second.Get_ID()==9) L_arm=iter->second;
            if(iter->second.Get_ID()==15) R_arm=iter->second;
        }
        
        if (R_arm.Get_y()>Torso.Get_y() || L_arm.Get_y()>Torso.Get_y())
        {
            speed=0;
            if(R_arm.Get_y()>Torso.Get_y())
            {
				veces_r++;
				if(veces_r>=10)
				{
                patrol->getInstance().Sintetizer.set_Phrase("i learnd this place as DININGROOM");
				sleep(2);
				veces_r=-1000;
				}
            }
            else
            {
				veces_l++;
				if(veces_l>=10)
				{
                patrol->getInstance().Sintetizer.set_Phrase("i learnd this place as KITCHEN");
				sleep(2);
				veces_l=-1000;
				}
            }
        }
        
        
        
        else 
        {

			if(veces_l>=0)
			{
				veces_l=0;
			}
			if(veces_r>=0)
			{
				veces_r=0;
			}
                if (Torso.Get_z()<600  )
                {
                    speed=-200;
                }
                else 
                {
                    if (Torso.Get_z()>1200&& Torso.Get_z()<2500 )
                    {
                        // robot.move(10*15);
                        speed=Torso.Get_z()-1200;
                        
                    }
                }
                
                if (Torso.Get_x() > 70 )
                {
                    rot=5;
                    
                }
                else if (Torso.Get_x() < -70 )
                {
                    rot=-5;
                    
                }
                else
                {
                    rot=0;
                } 
            
        }
        
        
        robot.setVel(speed);
        robot.setDeltaHeading(rot);
        
    }
    

}

void Follower::run()
{
	pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}

void Follower::stop()
{
	
}



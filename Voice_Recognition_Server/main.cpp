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
#include <map>
#include <pthread.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>

//#include "../Gui/gui.h"
//#include "../Gui/video.h"
#include "../Plugin_API/pluginapi.h"
#include "../Shared_memory/Robot.h"
#include "../Shared_memory/Location.h"

//#include "../Gui/input_singleton.h"
//#include "../Gui/gui.h"
//#include "../Gui/tab.h"
//#include "../Gui/console.h"

//#include "../MDP/MDP.h"

#include <cstdlib>
#include <iostream>

#include "Thread.h"
#include "NetThread.h"
#include "gsNetPoll_g++.h"
#include <alsa/asoundlib.h>
#include <alsa/pcm.h>
#include <math.h>
#define BUFFER_LEN 88000

Robot *patrol;


#define PLUGIN_NAME "VoiceRecognitionServer"

static char device[] = "default";
snd_output_t *output = NULL;
float buffer [BUFFER_LEN];
int numberOfUsersToLearn=0;

void TextToSpeech(string speech, unsigned int counter)
{
	patrol->getInstance().Sintetizer.set_Phrase(speech);
	sleep(counter);
}

void CleanSpeech(NetThread *net)
{
	net->messages.clear();	
	net->Write("REQstartClean");
	net->Read();
	net->GetIncoming();
}

void beepStartSpeech(void)
{
    int err;
    int j,k;

    int f = 2000;                //frequency 1
    int fs = 48000;             //sampling frequency

    snd_pcm_t *handle;
    snd_pcm_sframes_t frames;


    // ERROR HANDLINGii

    if ((err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
            printf("Playback open error: %s\n", snd_strerror(err));
            exit(EXIT_FAILURE);
    }

    if ((err = snd_pcm_set_params(handle, SND_PCM_FORMAT_FLOAT, SND_PCM_ACCESS_RW_INTERLEAVED, 1, 48000, 1, 500000)) < 0) {   
            printf("Playback open error: %s\n", snd_strerror(err));
            exit(EXIT_FAILURE);
    }

    // SINE WAVE
    printf("Sine tone at %dHz", f);

        for (k = 0; k < BUFFER_LEN; k++)
            buffer[k] = (1 / ((float) exp(2*k / 880.0))) * sin(2 * M_PI * f / fs * k);                 //sine wave value generation                        

        for (j = 0; j < 1; j++)
            frames = snd_pcm_writei(handle, buffer, BUFFER_LEN);    //sending values to sound driver
            

    snd_pcm_close(handle);

}

string LoopRecognize(char *request, NetThread *net, string subject)
{
	string Speech = "";
	string conf = "no";
	int counter = 0;
	string received = "";
	string repeat = "";
	
			do{
				net->messages.clear();	
				net->Write(request);
				net->Read();
				usleep(500*1000);
					
				if (net->GetStatus()>0){
					
					Speech = net->GetIncoming();
					net->messages.clear();
					while(Speech =="?" || Speech =="??" || Speech =="" || Speech =="yes" || Speech =="no")
					{
						usleep(500*1000);
						net->Write("REQsendingWord");
												
						net->Read();
						if (net->GetStatus()>0){
						Speech = "";  
						Speech = net->GetIncoming();
						net->messages.clear();
						}
						
						counter++;
							
							if(counter == 10)
							{
								repeat = "Repeat please the "+ subject;
								TextToSpeech(repeat,2);
								counter = 1;

							}
							if(counter == 1)
							{
								beepStartSpeech();
							}
					}
					
					counter = 2;
					sleep(1);
					// Here makes a confirmation
					std::cout << "Makes a confirmation"<<std::endl;
					// Here makes a request from confirmation
					net->messages.clear();
					net->Write("REQstartConfirmation");
					received = "Did you tell me " + Speech + "?";
					TextToSpeech(received,round(received.length()/12));
					usleep(500*1000);
					net->Read();
					conf = net->GetIncoming();
					conf = "?";
					net->messages.clear();
					CleanSpeech(net);
					
						while(conf =="?" || conf =="??" || conf =="")
						{							
							usleep(500*1000);
							net->Write("REQsendingWord");
							
							net->Read();
							if (net->GetStatus()>0){
							conf = "";  
							conf = net->GetIncoming();
							net->messages.clear();
							}
							
							counter++;
							
							if(counter == 14)
							{
								TextToSpeech(received,2);
								counter = 3;
							}
							if(counter == 3)
							{
								beepStartSpeech();
							}
						}
						if(conf == "no")
						{
							repeat = "Can you repeat please the "+subject;
							TextToSpeech(repeat,2);
							counter = 0;
						}
					}
			}while(conf=="no");
			
			CleanSpeech(net);
			net->messages.clear();
			
			std::cout << "REQUEST: " << request << "WORD: " << Speech <<std::endl;
			return Speech;
}

string LoopPhrase(char *request, NetThread *net)
{
	string Speech = "";
	int counter = 0;
	string received = "";
	
			do{
				net->messages.clear();	
				net->Write(request);
				net->Read();
				usleep(500*1000);
					
				if (net->GetStatus()>0){
					
					Speech = net->GetIncoming();
					net->messages.clear();
					
					while(Speech =="?" || Speech =="??" || Speech =="")
					{
						usleep(500*1000);
						net->Write("REQsendingPhrases");
												
						net->Read();
						if (net->GetStatus()>0){
						Speech = "";  
						Speech = net->GetIncoming();
						net->messages.clear();
						}
						
						counter++;
						
							if(counter == 1)
							{
								beepStartSpeech();
								//sleep(5);
								counter = -12;
							}
					}
					
					counter = 0;
					sleep(1);
					// Here makes a confirmation
					std::cout << "Makes a confirmation"<<std::endl;
					// Here makes a request from confirmation
					net->messages.clear();
					//received =  Speech + "?";
					//TextToSpeech(received,round(received.length()/12));
					
					}
			}while(Speech =="?" || Speech =="??" || Speech =="");
			
			CleanSpeech(net);
			net->messages.clear();
			
			std::cout << "REQUEST: " << request << "WORD: " << Speech <<std::endl;
			return Speech;
}

string LoopStart(char *request, NetThread *net)
{
	string Speech = "";
	int counter = 0;
	string received = "";
	string repeat = "";
	
			do{
				net->messages.clear();	
				net->Write(request);
				net->Read();
				usleep(1000*1000);
					
				if (net->GetStatus()>0){
					
					Speech = net->GetIncoming();
					net->messages.clear();
					
					while(counter<=6)
					{
						usleep(500*1000);
						net->Write("REQsendingWord");
												
						net->Read();
						if (net->GetStatus()>0)
						{
							Speech = "";  
							Speech = net->GetIncoming();
							net->messages.clear();
						}
						if(Speech == "?")
						{
							counter++;
						}
						
					}
										
					}
			}while(counter<=6);
			
			return Speech;
}



/**
 * @brief Net_server plugin
 *
 * This plugin is a network server that accepts client conections to comunicate with other programs (running on the same computer or over the network)
 * @author     Patrick Heyer, ickhey@prodigy.net.mx, Juan Herrera juan_antonio_@hotmail.com, Manuel Oropeza zodiacanimations@msn.com
 * @date       jul 04, 2012
 * @version    1.0
 **/
class VoiceRecognitionServer : public IPlugin
{
public:
    void Main();
    void stop();
    void run();
	//void LoopRecognize(char *,string,NetThread *);
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new VoiceRecognitionServer;
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



void VoiceRecognitionServer::Main()
{
    /**
     * \sa print
     **/
    //Gui::getInstance();
    //pluginTab = new Tab("Net_server");
    //cons= new Console(WIDTH-600,HEIGHT*.02,600,HEIGHT/2, "net_console", pluginTab);

    NetThread *net = new NetThread();
    string speech= "";

    net->SetOutputStream(stdout);
    net->OpenReadPort(2070);
    net->start();
    net->Accept();
    sleep(10);

    string line;
    ifstream NamesFile("../data/Names");
    if(NamesFile)
    {
    vector<string> names;
    
    while (getline(NamesFile, line))
    {
        names.push_back(line);
    }
    }

    ifstream ObjectsFile("../data/Objects");
    if(ObjectsFile)
    {
    vector<string> objects;
    while (getline(ObjectsFile, line))
    {
        objects.push_back(line);
    }
    }
    
    ifstream DestinationsFile("../data/Destinations");
    if(DestinationsFile)
    {
    vector<string> Destinations;
    while (getline(DestinationsFile, line))
    {
        Destinations.push_back(line);
    }
    }
 
    string accion;
  
	
	for (;;){
		string action = patrol->getInstance().get_Action();
		
		
		if (action =="requestOrderRestaurant"){			
			
			string Location = "";
			string ObjectA = "";
			string ObjectB = "";
			TextToSpeech("I am ready to take your order",3);
			TextToSpeech("please tell me the first object",3);
			ObjectA = LoopRecognize("REQstartRestaurant",net,"name of the first object"); //confirmando
			sleep(5);
			TextToSpeech("i will bring "+ObjectA,5);
			
			TextToSpeech("please tell me the second object",3);
			ObjectB = LoopRecognize("REQstartRestaurant",net,"name of the second object"); //confirmando
			sleep(5);
			TextToSpeech("i will bring "+ObjectA,4);
			
			TextToSpeech("please tell me where to take"+ObjectA+" and "+ ObjectB,6);
			Location = LoopRecognize("REQstartRestaurant",net,"name of the location"); //confirmando
			sleep(5);
			TextToSpeech("i will bring "+ObjectA+"and"+ ObjectB + " to "+Location ,6);
			

	
		}
	}
    usleep(100);
}



void VoiceRecognitionServer::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}

void VoiceRecognitionServer::stop()
{

}


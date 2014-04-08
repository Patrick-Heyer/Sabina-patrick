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
#include <cstdlib>


#define PLUGIN_NAME "NavigationClient"

#include "../Plugin_API/pluginapi.h"
#include "../Shared_memory/Robot.h"
#include "../Shared_memory/Location.h"

#include "ArNetworking.h"


char textData[64];
char currentGoal[64];
bool openDoor = false;
// int robotXPosition = 0;
// int robotYPosition = 0;
// double robotThPosition = 0.0;
Robot *patrol;
bool voiceRecognizerSelection=false;
Location tempLocation;


void handlePathPlannerStatus(ArNetPacket *packet) {
    char buf[64];

    packet -> bufToStr(buf, 63);
    //printf(".. Path planner status: \"%s\"\n", buf);
}


void handleGoalName(ArNetPacket* packet) {

    packet->bufToStr(currentGoal, 63);
    //printf(".. Current goal: \"%s\"\n", currentGoal);

}

void handleRobotUpdate(ArNetPacket* packet) {

    char buf[64];

    packet->bufToStr(textData, 63);
    //printf("... Robot server status: \"%s\"\n", textData);

    packet->bufToStr(buf, 63);
    packet->bufToByte2();

//   robotXPosition = packet->bufToByte4();
//   robotYPosition = packet->bufToByte4();
//   robotThPosition = packet->bufToByte4();

    tempLocation.set_X(packet->bufToByte4());
    tempLocation.set_Y(packet->bufToByte4());
    tempLocation.set_Angle(packet->bufToByte4());
    patrol->getInstance().setPosition(tempLocation);


// printf("Robot position: (%d, %d, %d)\n", robotXPosition, robotYPosition, robotThPosition);

}


void handleGoalList(ArNetPacket *packet) {

    printf(".. Server has these goals:\n");
    char goal[256];

    for(int i = 0; packet -> getReadLength() < packet -> getLength(); i++) 
    {
        packet -> bufToStr(goal, 255);
        if(strlen(goal) == 0)
            return;

        //printf("      %s\n", goal);
    }
}

void handleSensorCurrent(ArNetPacket *packet) {
    char buf[64];
    int readings, nreadings;
    openDoor = false;
    nreadings = packet->bufToByte2();
    int laserXReadings[nreadings];
    int laserYreadings[nreadings];

    float distance = 0.0;
    float minDistance = 5000.0;

    // nreadings = packet -> bufToByte2();
    //printf("Number of readings: %d\n", nreadings);

    packet -> bufToStr(buf, 63);
    //printf("Sensor's Name: %s\n", buf);

    for (readings = 0; readings < nreadings; readings++) {
        laserXReadings[readings] = packet->bufToByte4();
        laserYreadings[readings] = packet->bufToByte4();
        // printf("(%d, %d)  ", packet -> bufToByte4(), packet -> bufToByte4());
    }

    for (readings = 80; readings <= 100; readings++) {
        distance = sqrt(pow((laserXReadings[readings] - tempLocation.get_X()), 2) + pow((laserYreadings[readings] -tempLocation.get_Y()), 2));

        if (distance <= minDistance)
            minDistance = distance;
    }

    if (minDistance > 1800) {
        openDoor = true;
        printf("Door is open\n");
        //printf("minDistance (%f), Distance (%f)", minDistance, distance);
    }
    else {
        openDoor = false;
        printf("Door is closed\n");
        printf("\n");
    }
}

void handleSensorList(ArNetPacket *packet) {
    char buf[64];
    int i;

    printf("Number of sensors: %d\n", packet -> bufToByte2());

    printf(".. Server has these sensors:\n");

    for (i = 0; packet -> getDataReadLength() < packet -> getLength(); i++) {
        packet -> bufToStr(buf, 63);

        if (strlen(buf) == 0)
            return;

        printf("	%s\n", buf);
    }
}

void handleLocState(ArNetPacket *packet) {

    printf("Localization state: %d\n", packet -> bufToUByte());
    printf("Localization Score: %d\n", packet -> bufToUByte2());
}

void navigateTo(string destination, ArClientBase *client) {

    string speech = "Starting navigation to " + destination;
    patrol->getInstance().Sintetizer.set_Phrase(speech);
    client->requestOnceWithString("gotoGoal", destination.c_str());

    sleep(10);
    printf("Before while Navigation");
    while (strncmp(textData, "Arrived at", 10) != 0) {
        sleep(10);
    }
    sleep(2);
}



/**
 * @brief NavigationClient plugin
 *
 * This plugin is a network server that accepts client conections to comunicate with other programs (running on the same computer or over the network)
 * @author     Patrick Heyer, patrickhey@prodigy.net.mx, Juan Herrera juan_antonio_@hotmail.com, Manuel Oropeza zodiacanimations@msn.com
 * @date       jul 04, 2012
 * @version    1.0
 **/
class NavigationClient : public IPlugin
{
public:
    void Main();
    void run();
    void stop();
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new NavigationClient;
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

void NavigationClient::Main()
{

    std::cout << "Navigation client started" << std::endl;
    int argsNumber;
    argsNumber = 3;
    char *arguments[argsNumber];
    arguments[0]="./arg_test";
    arguments[1]="-host";
    arguments[2]="10.42.0.1";

    int robotCase, counter;

    ArClientBase client;
    ArPose pose(0, 0, 0);
    ArPose GoToPose(3000.0, 0.0, -90.0);
    ArNetPacket posePacket, directGoToPosePacket;

    ArArgumentParser parser(&argsNumber, arguments);
    posePacket.empty();

    ArClientSimpleConnector clientConnector(&parser);
    parser.loadDefaultArguments();

    openDoor = false;
    if (!clientConnector.parseArgs() || !parser.checkHelpAndWarnUnparsed()) {
        clientConnector.logOptions();
        exit(0);
    }

    printf("Connecting...\n");
    if (!clientConnector.connectClient(&client)) {
        if (client.wasRejected())
            printf("Server rejected connection, exiting\n");
        else
            printf("Could not connect to server, exiting\n");

        exit(1);
    }

    printf("Connected to server.\n");
    client.addHandler("pathPlannerStatus", new ArGlobalFunctor1<ArNetPacket*>(&handlePathPlannerStatus));
    client.addHandler("update", new ArGlobalFunctor1<ArNetPacket*>(&handleRobotUpdate));
    client.addHandler("goalName", new ArGlobalFunctor1<ArNetPacket*>(&handleGoalName));
    client.addHandler("getGoals", new  ArGlobalFunctor1<ArNetPacket*>(&handleGoalList));
    client.addHandler("getSensorCurrent", new ArGlobalFunctor1<ArNetPacket*>(&handleSensorCurrent));
    client.addHandler("getSensorList", new ArGlobalFunctor1<ArNetPacket*>(&handleSensorList));
    client.addHandler("getLocState", new ArGlobalFunctor1<ArNetPacket*>(&handleLocState));

    client.runAsync();

    client.requestOnce("getGoals");
    client.requestOnce("getSensorList");
    client.requestOnceWithString("getSensorCurrent", "lms2xx_1");
    // client.request("pathPlannerStatus", 5000);
    client.request("goalName", 1000);
    client.request("update", 1000);
    client.requestOnce("getLocState");

    for(;;) {
        string action = patrol->getInstance().get_Action();

        if (action=="wait_for_door")
        {

            do
            {
                ArUtil::sleep(2000);
                client.requestOnceWithString("getSensorCurrent", "lms2xx_1");

            } while (!openDoor);
            patrol->getInstance().set_Action("none");
        }

        if(action=="localize")
        {
            if (client.dataExists("localizeToPose")) {
                printf(".. Server does have \"localizeToPose\" request.\n");
                posePacket.byte4ToBuf(pose.getX());
                posePacket.byte4ToBuf(pose.getY());
                posePacket.byte4ToBuf(pose.getTh());
                client.requestOnce("localizeToPose", &posePacket);
                ArUtil::sleep(1500);
            }
            else
            {
                printf(".. Server doesn't have that request.\n");
            }
            patrol->getInstance().set_Action("none");
        }

        if (action=="navigate") {
            cout<<"Starting: "<< action << "STATE in NavigationClient"<<endl;
            navigateTo(patrol->getInstance().get_Current_destination(), &client);
            patrol->getInstance().set_Action("none");
        }
        
        ArUtil::sleep(2000);
	client.requestOnceWithString("getSensorCurrent", "lms2xx_1");


    }
    return ;

}

void NavigationClient::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}

void NavigationClient::stop()
{

}

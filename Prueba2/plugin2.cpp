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
#include <pthread.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include "../Gui/gui.h"
#include "../Gui/video.h"
#include "../Plugin_API/pluginapi.h"
#include "../Shared_memory/Robot.h"

#include "../Gui/input_singleton.h"
#include "../Gui/gui.h"
#include "../Gui/tab.h"
#include "../Gui/console.h"
#include "../Gui/video.h"
#include "../Gui/kinect.h"

Robot *patrol;

video *vid;
tab *current_tab;


#define WIDTH 1440
#define HEIGHT 900

#include <opencv/cv.h>
#include <opencv/highgui.h>

#define PLUGIN_NAME "Vision1"

class Vision1 : public IPlugin
{
public:
    void Main();
    void run();
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Vision1;
}

PLUGIN_FUNC void DestroyRenderer(IPlugin *r)
{
    delete r;
}

PLUGIN_DISPLAY_NAME(PLUGIN_NAME);

PLUGIN_INIT()
{
    // register our new renderer
    std::cout << "PLUGIN_INIT" << std::endl;
    RegisterPlugin(PLUGIN_NAME, CreatePlugin, DestroyRenderer);
    return 0;
}

CvHaarClassifierCascade * cascade = 0;  // the face detector
CvMemStorage * pStorage = 0;             // memory for detector to use
CvSeq * pFaceRectSeq;                    // memory-access interface

CvRect * detectFace(IplImage * pImg,int *how_many){
    CvRect* r;
    int scale = 1,i;
    int minFaceSize = pImg->width / 5;
    pFaceRectSeq = cvHaarDetectObjects(pImg, cascade, pStorage,1.1,4,CV_HAAR_DO_CANNY_PRUNING,cvSize(minFaceSize, minFaceSize));
    if(pFaceRectSeq->total){
        r = (CvRect*)cvGetSeqElem(pFaceRectSeq, 0);
        cvRectangle(pImg,cvPoint(r->x,r->y),cvPoint(r->x + r->width ,r->y + r->height) , CV_RGB(0,0,255) , 3, 8, 0 );
        *how_many=1;
    }
    else  *how_many=0;
    return r;
}

int initFaceDet(const char * haarCascadePath){
    if( !(pStorage = cvCreateMemStorage(0)) ){
        fprintf(stderr, "Can\'t allocate memory for face detection\n");
        return 0;
    }
    cascade = (CvHaarClassifierCascade *)cvLoad( haarCascadePath, 0, 0, 0 );
    if( !cascade ){
        fprintf(stderr, "Can\'t load Haar classifier face cascade from");
        return 0;
    }
    
    return 1;
}

            
void Vision1::Main()
{
    int polo=0;
    GUI::GetInstance();
    IplImage *frame;
    IplImage * pGrayImg = 0;
    current_tab = new tab("Vision1");
    vid = new video(0,HEIGHT*.02,WIDTH/2,HEIGHT/2,"Vision1", current_tab);
    initFaceDet("haarcascade_frontalface_default.xml");
    while (true)
    {
        frame=cvCloneImage( patrol->get_Instance().get_image());

        pGrayImg = cvCreateImage( cvSize(frame->width, frame->height), frame->depth, 1 );
        cvCvtColor(frame, pGrayImg, CV_RGB2GRAY);
        vid->SetImage(pGrayImg);
        cvReleaseImage(&pGrayImg);
        cvReleaseImage(&frame);
    }
    
}

void Vision1::run()
{
    
    
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}



/*
 * Copyright (C) 2011, Mathieu Labbe - IntRoLab - Universite de Sherbrooke
 */

#include <stdio.h>

#include "../Plugin_API/pluginapi.h"
#include "../Shared_memory/Robot.h"
#include "../MDP/MDP.h"
#include "../Gui/gui.h"
#include "../Gui/console.h"
#include "../Gui/video.h"

#include "../Kinect/Ni_Device.h"

#include "facedetect.h"
#include "recognizer.hpp"

// OpenCV stuff
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

Robot *patrol;

Tab *pluginTab;

#define PLUGIN_NAME "Vision_Objects"

/**
 * @brief Vision_Objects plugin
 *
 * This plugin is used for object recognition using kinects depth map to reduce areas of interest for sift search
 * @author     Patrick Heyer, patrickhey@prodigy.net.mx
 * @date       jul 04, 2012
 * @version    1.0
 **/
class Vision_Objects : public IPlugin
{
public:
    void Main();
    void stop();
    void run();
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Vision_Objects;
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

void Vision_Objects::Main()
{
    Gui::getInstance();
    pluginTab = new Tab("Vision_Objects");

    std::string accion;

    cv::Mat kinect_rgb;
    cv::Mat kinect_gray;
    cv::Mat kinect_depth;

    cv::Mat contours_img;
    cv::Mat cutout;


    initFaceDet("../data/haarcascade_frontalface_default.xml");
    initEyeDet("../data/parojos.xml");
    initEyeDetD("../data/ojoD.xml");
    initEyeDetI("../data/ojoI.xml");


    int conta=0;
    int conta_faces=0;
    int conta_faces_recognized=0;
    bool reload=true;

    OrbRecognizer OrbObject;

    OrbObject.setThreshold(0.8);
    OrbObject.setRecognitionTreshold(5);
    OrbObject.setMachine(3); // es SIFT.
    OrbObject.setDistance(1);

    patrol->getInstance().set_Action("aprender_persona");
    sleep(10);
    int tilt=-30;
    for (;;)
    {
      tilt=tilt++;
      if (tilt>30) tilt=-30;
      Ni_Device::getInstance().setTilt(tilt);
//       sleep(5);

        accion=patrol->getInstance().get_Action();
        if (accion=="reconocer_objeto")
        {
            std::stringstream ss;
            ss << "../data/Objects/" << patrol->getInstance().Microphone.get_Phrase() << "/";
            OrbObject.TrainingSet(ss.str().c_str());

            cv::RNG rng(12345);

            kinect_depth=patrol->getInstance().KINECT->get_depth();
            kinect_rgb = patrol->getInstance().KINECT->get_RGB();

            int threshold=50;
            cv::threshold(kinect_depth, kinect_depth, threshold, 255, cv::THRESH_TOZERO_INV );
            cv::threshold(kinect_depth, kinect_depth, threshold-5, 255, cv::THRESH_TOZERO_INV);

            cv::Mat destination;


            cv::resize(kinect_depth, destination, kinect_rgb.size(), 0, 0, cv::INTER_CUBIC);
// 	    cv::blur( kinect_depth, kinect_depth, cv::Size(3,3) );
//             cv::namedWindow("threshold", CV_WINDOW_AUTOSIZE);
//             cv::imshow("threshold", destination);

            vector<vector<cv::Point> > contours;
            vector<cv::Vec4i> hirarchy;

            cv::findContours(destination, contours, hirarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0,0));

            vector<vector<cv::Point> >contours_poly(contours.size());
            vector<cv::Rect> boundRect (contours.size());
            vector<cv::Point2f> center(contours.size());
            vector<float>radius(contours.size());

            for(int i=0; i< contours.size(); i++)
            {
                cv::approxPolyDP( cv::Mat(contours[i]), contours_poly[i], 3, true );
                boundRect[i]=cv::boundingRect( cv::Mat(contours_poly[i]) );
                cv::minEnclosingCircle( (cv::Mat)contours_poly[i], center[i], radius[i] );
            }


            cvtColor( kinect_rgb, kinect_gray, CV_RGB2GRAY );
            cv::GaussianBlur( kinect_gray, kinect_gray, cv::Size( 3, 3 ), 0, 0 );

            contours_img= cv::Mat::zeros( destination.size(), CV_8UC3);
            for (int i=0; i < contours.size(); i++)
            {
                cv::Scalar color= cv::Scalar(rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255) );
                cv::drawContours( destination, contours_poly, i, color, 1, 8, vector<cv::Vec4i>(), 0, cv::Point() );
                cv::rectangle(contours_img, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
                cv::circle(contours_img, center[i], (int)radius[i], color, 2, 8, 0);
                if( boundRect[i].height>64 && boundRect[i].height<512 && boundRect[i].width>64 && boundRect[i].width<512 )
                {
                    cutout=kinect_gray(boundRect[i]);
                    cv::imshow("cutout",cutout);
                    std::cout << "ultima altura " << boundRect[i].height << "ultimo ancho " << boundRect[i].width << std::endl;
                    cv::waitKey(100);
 
                    if(OrbObject.evalWindow(cutout))
                    {
                        cv::rectangle(kinect_rgb, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);                        ;
                        patrol->getInstance().object_X=center[i].x/2;
                        patrol->getInstance().object_Y=center[i].y/2.3333;
                        cv::destroyAllWindows();
                        patrol->getInstance().set_Action("none");
                    }
                    else {
                        std::cout << "NO ENCONTRE" << std::endl;
                    }
                }
            }
        }


        if (accion=="aprender_objeto")
        {

            cv::RNG rng(12345);

            kinect_depth=patrol->getInstance().KINECT->get_depth();
            kinect_rgb = patrol->getInstance().KINECT->get_RGB();

            int threshold=50;
            cv::threshold(kinect_depth, kinect_depth, threshold, 255, cv::THRESH_TOZERO_INV );
            cv::threshold(kinect_depth, kinect_depth, threshold-5, 255, cv::THRESH_TOZERO_INV);



            cv::Mat destination;
            cv::resize(kinect_depth, destination, kinect_rgb.size(), 0, 0, cv::INTER_CUBIC);

            vector<vector<cv::Point> > contours;
            vector<cv::Vec4i> hirarchy;

            cv::findContours(destination, contours, hirarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0,0));

            vector<vector<cv::Point> >contours_poly(contours.size());
            vector<cv::Rect> boundRect (contours.size());
            vector<cv::Point2f> center(contours.size());
            vector<float>radius(contours.size());

            for(int i=0; i< contours.size(); i++)
            {
                cv::approxPolyDP( cv::Mat(contours[i]), contours_poly[i], 3, true );
                boundRect[i]=cv::boundingRect( cv::Mat(contours_poly[i]) );
                cv::minEnclosingCircle( (cv::Mat)contours_poly[i], center[i], radius[i] );
            }

            cvtColor( kinect_rgb, kinect_gray, CV_RGB2GRAY );
            cv::GaussianBlur( kinect_gray, kinect_gray, cv::Size( 3, 3 ), 0, 0 );

            contours_img= cv::Mat::zeros( destination.size(), CV_8UC3);
            for (int i=0; i < contours.size(); i++)
            {
                cv::Scalar color= cv::Scalar(rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255) );
                cv::drawContours( destination, contours_poly, i, color, 1, 8, vector<cv::Vec4i>(), 0, cv::Point() );
                cv::rectangle(contours_img, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
                cv::circle(contours_img, center[i], (int)radius[i], color, 2, 8, 0);
                if( boundRect[i].height>64 && boundRect[i].height<512 && boundRect[i].width>64 && boundRect[i].width<512 )
                {
                    cutout=kinect_gray(boundRect[i]);
                    std::stringstream pp;
                    pp<< "../data/training/crop" << conta << ".png";
                    conta++;
                    cv::imwrite(pp.str().c_str(), cutout);
                }
            }
        }

        if (accion=="aprender_persona")
        {
	  
            CvRect * pFaceRect = 0;
            int how_many;

            IplImage *im_rgb  = patrol->getInstance().KINECT->get_RGB();
            IplImage *im_gray = cvCreateImage(cvGetSize(im_rgb),IPL_DEPTH_8U,1);
            cvCvtColor(im_rgb,im_gray,CV_RGB2GRAY);


            pFaceRect = detectFace(im_gray,&how_many);

            if(how_many) {
		patrol->getInstance().Sintetizer.set_Phrase("hello there i will try to memorize your face");
		sleep(4);
                IplImage *face  = cvCreateImage( cvSize(pFaceRect->width,pFaceRect->height),8,1);
                std::cout << "i see " << how_many << " faces" << std::endl;

                cvSetImageROI(im_gray, *pFaceRect );
                cvCopy(im_gray,face,0);
                cvResetImageROI(im_gray);

                cv::Mat output;
                output=face;
                std::cout << "Hay alguien " << std::endl;
                std::stringstream pp;
                pp<< "../data/Faces/pos/crop" << conta_faces << ".png";
                conta_faces++;
                cv::imwrite(pp.str().c_str(), output);
                cvReleaseImage(&face);
// 		if(conta_faces>=5)
// 		{
// 		  conta_faces=0;
		patrol->getInstance().set_Action("reconocer_persona");
		patrol->getInstance().Sintetizer.set_Phrase("hello there i memorized your face");
		sleep(4);
		  
// 		}
		
            }
        }

        if (accion=="reconocer_persona")
        {
	  
            if(reload)
            {
                OrbObject.TrainingSet("../data/Faces/");
                reload=false;
            }

            cv::RNG rng(12345);
            CvRect * pFaceRect = 0;
            int how_many;

            IplImage *im_rgb=patrol->getInstance().KINECT->get_RGB();
            IplImage *im_gray = cvCreateImage(cvGetSize(im_rgb),IPL_DEPTH_8U,1);
            cvCvtColor(im_rgb,im_gray,CV_RGB2GRAY);


            pFaceRect = detectFace(im_gray,&how_many);

            if(how_many) {
                IplImage *face  = cvCreateImage( cvSize(pFaceRect->width,pFaceRect->height),8,1);
                std::cout << "i see " << how_many << " faces" << std::endl;

                cvSetImageROI(im_gray, *pFaceRect );
                cvCopy(im_gray,face,0);
                cvResetImageROI(im_gray);


                cv::Mat output;
                output=face;

                if(OrbObject.evalWindow(output))
                {
                    std::cout << "Creo que te conozco" << std::endl;
                    conta_faces_recognized++;
                    if(conta_faces_recognized)
                    {
                        reload=true;
			conta_faces_recognized=0;
			patrol->getInstance().Sintetizer.set_Phrase("hello there i remember you");
			sleep(4);
			std::cout << "Hay alguien que conozco" << std::endl;
			sleep(30);
                        patrol->getInstance().set_Action("aprender_persona");
                    }
                }
                else
		{
		  conta_faces_recognized=0;
		}
                cvReleaseImage(&face);
            }
	    cvReleaseImage(&im_gray);
        }
    }
}



void Vision_Objects::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}

void Vision_Objects::stop()
{

}

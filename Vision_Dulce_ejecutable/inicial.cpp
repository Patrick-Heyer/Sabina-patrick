// g++ `pkg-config --cflags opencv` -o inicial inicial.cpp classifiers.cpp classifiers.hpp cvtexture_demo.cpp cvtexture_demo.hpp `pkg-config --libs opencv`
#include "Thread.h"
#include "NetThread.h"
#include "gsNetPoll_g++.h"


#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include "stdio.h" 

#include "classifiers.hpp"
#include "cvtexture_demo.hpp"

using namespace cv;
using namespace std;

IplImage* skipNFrames(CvCapture* capture, int n);

//int main(int argc, char** argv){
int main(int argc, char** argv){
 
  //***********************************
  //especificar cuantos clasificadores
  //archivo objects.txt debe coincidir con los conceptos aprendidos
  // ARCHIVO PATRRICK
  int UMB_NCLUST = 10; //20  //Número mínimo de elementos que debe tener el cluster para ser convertido a ventana.
  float umHuerfanos = 20; // salio +o- con 20
  float umDist = 15;  //  //Umbral: Criterio de Paro para el k-means jerarquico. 15
  
  int count = 0;
  //int countFrame = 0;
  int n = 2;  // numero de clasificadores
  LOCAL local;
  GLOBAL global;  
  GLCM glcm;

  //IplImage* img = cvLoadImage("cap_0021.jpg", 1);
  string name;
  name = argv[1];
  
  RECOGNITION recognition(n, name);  // deben estar afuera del for que  manda las   imagenes
  
  //************************************************************************************************************************
//   CvCapture* capture = cvCaptureFromFile("sabina11.webm");
  CvCapture* capture = cvCaptureFromCAM(2);
//Capturing a frame:

IplImage* frame = 0; 
if(!cvGrabFrame(capture)){              // capture a frame 
    printf("Could not grab a frame\n\7");
    exit(0);
}
// frame =cvRetrieveFrame(capture);// retrieve the captured frame
// cvShowImage("image", frame);
// cvWaitKey(0);

 NetThread *net = new  NetThread();
    net->SetOutputStream(stdout);
	sleep(5);
    net->OpenOutputAddress("localhost", 2080);
int cuadros=0;
 do
    {
	  frame = skipNFrames(capture, 1);
	  Mat coordinate;
	  cout << "Buscando " << name << endl;
	  coordinate = recognition.foundObjects(frame, local, global, glcm, n, UMB_NCLUST, umHuerfanos, umDist); //dentro del for
	  //cout << "\t" << "c.y" << "\t" << "c.y" << "pt1.x" << "\t" << "pt1.y" << "\t" << "pt2.x" << "\t" << "pt2.y" << endl;
	  //cout << "coordinate " <<  coordinate << endl;
	  //if (coordinate.at<float>(0,0) != 0 || coordinate.at<float>(0,1) != 0 || coordinate.at<float>(0,2) || coordinate.at<float>(0,3)){

	    if ( coordinate.at<float>(0,2) != 0 || coordinate.at<float>(0,3) != 0){
	  count++;
	  if( count == 1){
	    cout << "count " << count << endl;
			  std::stringstream ss;
				ss << coordinate.at<float>(0,0);
				net->Write(ss.str().c_str());
				ss.str("");
				ss <<coordinate.at<float>(0,1);
				net->Write(ss.str().c_str());
				
				std::stringstream ff;
            ff << "echo  '" << "i found "<< name << "' | festival --tts &";
            std::string temp=ff.str();
            std::cout <<temp << std::endl;
            system (temp.c_str());
				count++;
				
				sleep(1);

	    break;
	  }
	  else
	  {
		 cuadros++; 
		 if (cuadros >50)
		 {
			 net->Write("NULL");
			break; 
		 }
	  }
	    }

        cvNamedWindow("frame", CV_WINDOW_AUTOSIZE);
// 	cout << "sali " << endl;
        cvShowImage("frame", frame);
        cvWaitKey(1);
	
	  if ( coordinate.at<float>(0,2) != 0 || coordinate.at<float>(0,3) != 0){
	    count++;	    
	    cvWaitKey(5000);
	    if ( count == 1){
	      break;
	    }
	  }
    } while( frame != NULL );

 cvReleaseCapture(&capture);
 cvDestroyWindow("frame");
 // cvReleaseImage(&frame);
  //************************************************************************************************************************
 
return 0;
}

IplImage* skipNFrames(CvCapture* capture, int n)
{
    for(int i = 0; i < n; ++i)
    {
        if(cvQueryFrame(capture) == NULL)
        {
            return NULL;
        }
    }

    return cvQueryFrame(capture);
}



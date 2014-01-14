#include "cv.h" 
 #include "highgui.h" 
 #include <stdio.h>  
 
#include "surfrecognizer.h"
 
 // A Simple Camera Capture Framework 
 int main() {
   
   SurfRecognizer recognizer;
//    SurfRecognizer recognizer2;
//    SurfRecognizer recognizer3;
   
  // string objeto_file("box.png");
   string objeto_file1("kinect.png");
   string objeto_file2("baco.png");
   string objeto_file3("coca.png");
   
   recognizer.setObjectFileName(objeto_file2);
   recognizer.setDisplaying(true);
   recognizer.setRecognitionTreshold(15);
   recognizer.loadObjectFile();
   recognizer.displayObject();
   
   CvCapture* capture = cvCaptureFromCAM( CV_CAP_ANY );
   if ( !capture ) {
     fprintf( stderr, "ERROR: capture is NULL \n" );
     getchar();
     return -1;
   }
   // Create a window in which the captured images will be presented
   cvNamedWindow( "mywindow", CV_WINDOW_AUTOSIZE );
   // Show the image captured from the camera in the window and repeat
   while ( 1 ) {
     // Get one frame
     IplImage* frame = cvQueryFrame( capture );
     if ( !frame ) {
       fprintf( stderr, "ERROR: frame is null...\n" );
       getchar();
       break;
     }
     
     cvShowImage( "mywindow", frame );
     
     
     // Do not release the frame!
     //If ESC key pressed, Key=0x10001B under OpenCV 0.9.7(linux version),
     //remove higher bits using AND operator
     if ( (cvWaitKey(10) & 255) == 27 ) break;
     
     
     recognizer.recongnizeObjectInImage(frame);
     //recognizer2.recongnizeObjectInImage(frame);
     
   }
   // Release the capture device housekeeping
   cvReleaseCapture( &capture );
   cvDestroyWindow( "mywindow" );
   return 0;
 }
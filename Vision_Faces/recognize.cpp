#include "sift.h"
#include "imgfeatures.h"
#include "kdtree.h"
#include "utils.h"
#include "xform.h"
#include "cv.h"
#include "highgui.h"
#include "load.h"
#include "facedetect.h"
#include <stdarg.h>
#include <math.h>

#define KDTREE_BBF_MAX_NN_CHKS 200
#define NN_SQ_DIST_RATIO_THR 0.49

#include "../Shared_memory/Robot.h"

Robot *polo;


int recognize (IplImage *imagen) {
    CvRect * pFaceRect = 0;
    IplImage *clone;

    int how_many,temp,count;
    //cvNamedWindow("reco",1);
    IniciaProbabilidades();
    // fprintf( stdout, "(recognize.cpp) recognize function..\n");
    count=0;
    int persona=0;


    clone = cvCloneImage(imagen);

    pFaceRect = detectFace(clone,&how_many);
    //cvShowImage("reco",imageR);
    if (how_many) {
        IplImage *tmp = cvCreateImage( cvSize(pFaceRect->width,pFaceRect->height),8,3);
        IplImage *face  = cvCreateImage( cvSize(pFaceRect->width,pFaceRect->height),8,1);
        cvSetImageROI(clone, *pFaceRect );
        cvCopy(clone,tmp,0);
        cvResetImageROI(clone);
        cvCvtColor(tmp,face,CV_RGB2GRAY);
        int temp=ComparaBD(face,count++) ;
        //  printf("(Reconoci a  %d)\n",temp);
        if (temp!=0) {
            persona=temp;
        }
        cvReleaseImage(&tmp);
        cvReleaseImage(&face);
    }
    else fprintf(stdout,"No vi a naiden \n");




    cvReleaseImage(&clone);
    //cvSaveImage("saved.jpg",clone);
    //cvDestroyWindow("reco");
    //  fprintf( stdout, "(recognize.cpp) End recognize\n");
    return persona;
}





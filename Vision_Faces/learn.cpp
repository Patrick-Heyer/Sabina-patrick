
#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#include "load.h"
#include "facedetect.h"
#include <stdarg.h>
#include <math.h>

#include "../Shared_memory/Robot.h"

Robot *polito;

int learn (IplImage *imagen) {
    CvRect * pFaceRect = 0;
    IplImage *clone;
    int how_many,band;
    fprintf( stdout, "\nlearn function..");
    //cvNamedWindow("Learn",1);

    int pude=0;


    clone = cvCloneImage(imagen);


    pFaceRect = detectFace(clone,&how_many);
    //cvShowImage("Learn",clone);

    if(how_many) {
        IplImage *tmp = cvCreateImage( cvSize(pFaceRect->width,pFaceRect->height),8,3);
        IplImage *face  = cvCreateImage( cvSize(pFaceRect->width,pFaceRect->height),8,1);
        cvSetImageROI(clone, *pFaceRect );
        cvCopy(clone,tmp,0);
        cvResetImageROI(clone);
        cvCvtColor(tmp,face,CV_RGB2GRAY);
        fprintf(stdout,"\n\t Hay alguien ");
        pude=AprendeRostro(face);
        if(pude!=0) {
            printf("\n Aprendi a la persona que tenia enfrente");
        }
        cvReleaseImage(&tmp);
        cvReleaseImage(&face);
    }
    else fprintf(stdout,"\n\t No vi a alguien ");


    cvReleaseImage(&clone);
    //cvSaveImage("saved.jpg",clone);
    //cvDestroyWindow("Learn");
    fprintf( stdout, "\nEnd learn function..");
    return pude;
}


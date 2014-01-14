#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#define THRESHOLD 20


CvHaarClassifierCascade * cascade = 0;  // the face detector
CvHaarClassifierCascade * cascadeEye = 0;  // the face detector
CvHaarClassifierCascade * cascadeEyeD = 0;  // the face detector
CvHaarClassifierCascade * cascadeEyeI = 0;  // the face detector
CvMemStorage * pStorage = 0;             // memory for detector to use
CvMemStorage * pStorageEye = 0;             // memory for detector to use
CvMemStorage * pStorageEyeD = 0;             // memory for detector to use
CvMemStorage * pStorageEyeI = 0;             // memory for detector to use
CvSeq * pFaceRectSeq;                    // memory-access interface
CvSeq * pEyeRectSeq;                    // memory-access interface
CvSeq * pEyeRectSeqI;
CvSeq * pEyeRectSeqD;
CvPoint pt1, pt2,centro_izq,centro_der;;
CvRect * izq[5];
CvRect * der[5];
int found_d,found_i;
CvRect final_izq,final_der;



void cambio_izq(int x,int y,int x1,int y1,int ancho,int alto) {
    found_i++;
    centro_izq.x=x;
    centro_izq.y=y;
    final_izq=cvRect(x1,y1,ancho,alto);
}
void cambio_der(int x,int y,int x1,int y1,int ancho,int alto) {
    found_d++;
    centro_der.x=x;
    centro_der.y=y;
    final_der=cvRect(x1,y1,ancho,alto);
}

int initFaceDet(const char * haarCascadePath) {
    if( !(pStorage = cvCreateMemStorage(0)) ) {
        fprintf(stderr, "Can\'t allocate memory for face detection\n");
        return 0;
    }
    cascade = (CvHaarClassifierCascade *)cvLoad( haarCascadePath, 0, 0, 0 );
    if( !cascade ) {
        fprintf(stderr, "Can\'t load Haar classifier face cascade from");
        return 0;
    }

    return 1;
}

int initEyeDet(const char * haarCascadePath) {
    if( !(pStorageEye = cvCreateMemStorage(0)) ) {
        fprintf(stderr, "Can\'t allocate memory for eye detection\n");
        return 0;
    }
    cascadeEye = (CvHaarClassifierCascade *)cvLoad( haarCascadePath, 0, 0, 0 );
    if( !cascadeEye ) {
        fprintf(stderr, "Can\'t load Haar classifier eye cascade from");
        return 0;
    }
    // cvNamedWindow("ojos",1);
    // cvNamedWindow("orig",1);
    // cvNamedWindow("comp",1);
    return 1;
}

int initEyeDetD(const char * haarCascadePath) {
    if( !(pStorageEyeD = cvCreateMemStorage(0)) ) {
        fprintf(stderr, "Can\'t allocate memory for eye detection\n");
        return 0;
    }
    cascadeEyeD = (CvHaarClassifierCascade *)cvLoad( haarCascadePath, 0, 0, 0 );
    if( !cascadeEyeD ) {
        fprintf(stderr, "Can\'t load Haar classifier eye cascade from");
        return 0;
    }
    return 1;
}

int initEyeDetI(const char * haarCascadePath) {
    if( !(pStorageEyeI = cvCreateMemStorage(0)) ) {
        fprintf(stderr, "Can\'t allocate memory for eye detection\n");
        return 0;
    }
    cascadeEyeI = (CvHaarClassifierCascade *)cvLoad( haarCascadePath, 0, 0, 0 );
    if( !cascadeEyeI ) {
        fprintf(stderr, "Can\'t load Haar classifier eye cascade from");
        return 0;
    }
    return 1;
}

void closeFaceDet() {
    if(cascade) cvReleaseHaarClassifierCascade(&cascade);
    if(pStorage) cvReleaseMemStorage(&pStorage);
}
void closeEyeDet() {
    if(cascadeEye) cvReleaseHaarClassifierCascade(&cascadeEye);
    if(pStorageEye) cvReleaseMemStorage(&pStorageEye);
}
void closeEyesDet() {
    if(cascadeEyeD) cvReleaseHaarClassifierCascade(&cascadeEyeD);
    if(pStorageEyeD) cvReleaseMemStorage(&pStorageEyeD);
    if(cascadeEyeI) cvReleaseHaarClassifierCascade(&cascadeEyeI);
    if(pStorageEyeI) cvReleaseMemStorage(&pStorageEyeI);
}

int detecteOjos(IplImage * pImg) {
    CvRect* r;
    int scale = 1;
    int minFaceSize = pImg->width / 10;
    pFaceRectSeq = cvHaarDetectObjects(pImg, cascadeEye, pStorageEye,1.1,4,CV_HAAR_DO_CANNY_PRUNING,cvSize(minFaceSize, minFaceSize));
    if( pFaceRectSeq && pFaceRectSeq->total ) {
        r = (CvRect*)cvGetSeqElem(pFaceRectSeq, 0);
        return 1;
    }
    return 1;
}


CvRect * detectFace(IplImage * pImg,int *how_many) {
    CvRect* r=NULL;
    int scale = 1,i;
    int minFaceSize = pImg->width / 5;
    pFaceRectSeq = cvHaarDetectObjects(pImg, cascade, pStorage,1.1,4,CV_HAAR_DO_CANNY_PRUNING,cvSize(minFaceSize, minFaceSize));
    if(pFaceRectSeq->total) {
        r = (CvRect*)cvGetSeqElem(pFaceRectSeq, 0);
        cvRectangle(pImg,cvPoint(r->x,r->y),cvPoint(r->x + r->width ,r->y + r->height) , CV_RGB(255,0,0) , 3, 8, 0 );
        *how_many=1;
    }
    else  *how_many=0;
    return r;
}

int dist_euclidiana(CvPoint pt1,CvPoint pt2) {
    float x = pt1.x-pt2.x;
    float y = pt1.y-pt2.y;
    return (int)( sqrt( ( x*x ) + (y*y) ) );
}


int get_bigger(CvRect r1,CvRect *r2) {
    double a1=(double)( r1.width*r1.height) ;
    double a2=(double)( r2->width*r2->height );
    if(a1>=a2)
        return 1;
    else
        return 2;
}
IplImage* pImg;
IplImage* detectEyes(IplImage *pImg_orig,int pt[],int apun,int *detecte) {
    CvRect* r=0;
    pImg=cvCreateImage(cvSize(pImg_orig->width,pImg_orig->height),8,1);
    cvCopy(pImg_orig,pImg,0);
    int i;
    int minFaceSize = pImg->width / 10;
    int ancho=pImg->width/2;
    int alto=pImg->height/2;
    int centerX=0,centerY=0;
    int dist=0;
    int pte,prom,mitad,mayor,ancho1,ancho2,tercio,cuarto;
    *detecte=0;
    pEyeRectSeqD = cvHaarDetectObjects(pImg, cascadeEyeD, pStorageEyeD,1.1,6,CV_HAAR_DO_CANNY_PRUNING,cvSize(minFaceSize, minFaceSize));
    pEyeRectSeqI = cvHaarDetectObjects(pImg, cascadeEyeI, pStorageEyeI,1.1,6,CV_HAAR_DO_CANNY_PRUNING,cvSize(minFaceSize, minFaceSize));
    centro_izq.x=centro_izq.y=centro_der.x=centro_der.y=found_i=found_d=0;
    final_izq = cvRect(0,0,1,1);
    final_der = cvRect(0,0,1,1);
    for( i = 0; i < (pEyeRectSeqD ? pEyeRectSeqD->total : 0); i++ ) {
        r = (CvRect*)cvGetSeqElem( pEyeRectSeqD, i );
        centerX = r->x+ (r->width/2) ;
        centerY = r->y+ (r->height/2);
        if(centerY < alto ) {
            if(centerX<ancho) {                //candidato a ojo izq
                if( dist_euclidiana(centro_izq,cvPoint(centerX,centerY)) < THRESHOLD ) {
                    if( get_bigger(final_izq,r) > 1)
                        cambio_izq(centerX,centerY,r->x,r->y,r->width,r->height);
                }
                else {
                    if(centerY > (alto/2) )
                        cambio_izq(centerX,centerY,r->x,r->y,r->width,r->height);
                }
            }
            else {                            //candidato a derecho
                if( dist_euclidiana(centro_der,cvPoint(centerX,centerY)) < THRESHOLD ) {
                    if(get_bigger(final_der,r) > 1)
                        cambio_der(centerX,centerY,r->x,r->y,r->width,r->height);
                }
                else {
                    if(centerY > (alto/2) )
                        cambio_der(centerX,centerY,r->x,r->y,r->width,r->height);
                }
            }
        }
    }
    centerX=centerY=dist=0;
    for( i = 0; i < (pEyeRectSeqI ? pEyeRectSeqI->total : 0); i++ ) {
        r = (CvRect*)cvGetSeqElem( pEyeRectSeqI, i );
        centerX = r->x+ (r->width/2) ;
        centerY = r->y+ (r->height/2);
        if(centerY < alto ) {
            if(centerX<ancho) {                //candidato a ojo izq
                if( dist_euclidiana(centro_izq,cvPoint(centerX,centerY)) < THRESHOLD ) {
                    if( get_bigger(final_izq,r) > 1)
                        cambio_izq(centerX,centerY,r->x,r->y,r->width,r->height);
                }
                else {
                    if(centerY > (alto/2) )
                        cambio_izq(centerX,centerY,r->x,r->y,r->width,r->height);
                }
            }
            else {                             //candidato a derecho
                if( dist_euclidiana(centro_der,cvPoint(centerX,centerY)) < THRESHOLD ) {
                    if(get_bigger(final_der,r) > 1)
                        cambio_der(centerX,centerY,r->x,r->y,r->width,r->height);
                }
                else {
                    if(centerY > (alto/2) )
                        cambio_der(centerX,centerY,r->x,r->y,r->width,r->height);
                }
            }
        }
    }
    int ambos=1;
    if(found_i>0)
        if(found_d>0) {
            ancho1  = final_izq.x + final_izq.width;
            pte     = final_der.x - ancho1;
            prom    = cvRound ( (double) ((final_der.height + final_izq.height)/5) );
            ancho2  = final_der.x + final_der.width;
            tercio  = (final_izq.height < final_der.height) ? cvRound(final_izq.height/3):cvRound(final_izq.height/3);
            mayor   = (final_der.y < final_izq.y)  ? final_der.y:final_izq.y;
            if(pte >= 0) {
                mitad = cvRound((double)(pte/2));
                pte = ancho1 + mitad;
            }
            else {
                mitad = cvRound((double) ( (pte * -1)/2 ));
                pte = final_der.x + mitad;
            }
            pt[apun+0] = final_izq.y - prom;
            pt[apun+1] = final_der.y - prom;
            pt[apun+2] = final_izq.y + final_izq.height; // + tercio;
            pt[apun+3] = final_der.y + final_der.height; // + tercio;
            pt[apun+4] = mayor       + ancho2           - final_izq.x;
            pt[apun+5] = final_izq.x - prom;
            pt[apun+6] = final_izq.x;
            pt[apun+7] = pte;
            pt[apun+8] = ancho2;
            pt[apun+9] = ancho2      + prom;

            pt[apun+0] = (pt[apun+0] > 0) ? pt[apun+0]:0;
            pt[apun+1] = (pt[apun+1] > 0) ? pt[apun+1]:0;
            pt[apun+9] = (pt[apun+9] < pImg->width)  ? pt[apun+9] : pImg->width -1;
            pt[apun+4] = (pt[apun+4] < pImg->height) ? pt[apun+4] : pImg->height-1;

            cvRectangle( pImg, cvPoint(pt[apun+5]  ,pt[apun+0]), cvPoint(pt[apun+7],pt[apun+2]), CV_RGB(0,0,0), 2, 8, 0 );
            cvRectangle( pImg, cvPoint(pt[apun+7]+1,pt[apun+1]), cvPoint(pt[apun+9],pt[apun+3]), CV_RGB(0,0,0), 2, 8, 0 );
            cvRectangle( pImg, cvPoint(pt[apun+6]  ,pt[apun+2]), cvPoint(pt[apun+7],pt[apun+4]), CV_RGB(0,0,0), 2, 8, 0 );
            cvRectangle( pImg, cvPoint(pt[apun+7]+1,pt[apun+3]), cvPoint(pt[apun+8],pt[apun+4]), CV_RGB(0,0,0), 2, 8, 0 );
            *detecte=1;

            ambos=0;
        } // LOS 2 OJOS

        else { //SOLO EL OJO IZQ
            ancho1  = final_izq.x + final_izq.width;
            cuarto  = cvRound((double)(final_izq.width/5));
            pt[apun+0] = final_izq.y - cuarto;
            pt[apun+1] = final_izq.y - cuarto;
            pt[apun+2] = final_izq.y + final_izq.height; // + tercio;
            pt[apun+3] = final_izq.y + final_izq.height; // + tercio;
            pt[apun+4] = final_izq.y + (final_izq.width*2);
            pt[apun+5] = final_izq.x - cuarto;
            pt[apun+6] = final_izq.x;
            pt[apun+7] = ancho1;
            pt[apun+8] = ancho1 + cvRound((double)(final_izq.width * 0.666));
            pt[apun+9] = ancho1 + final_izq.width;

            pt[apun+0] = (pt[apun+0] > 0) ? pt[apun+0]:0;
            pt[apun+1] = (pt[apun+1] > 0) ? pt[apun+1]:0;
            pt[apun+5] = (pt[apun+5] > 0) ? pt[apun+5]:0;
            pt[apun+9] = (pt[apun+9] < pImg->width)  ? pt[apun+9] : pImg->width -1;
            pt[apun+4] = (pt[apun+4] < pImg->height) ? pt[apun+4] : pImg->height-1;

            cvRectangle( pImg, cvPoint(pt[apun+5]  ,pt[apun+0]), cvPoint(pt[apun+7],pt[apun+2]), CV_RGB(255,255,255), 2, 8, 0 );
            cvRectangle( pImg, cvPoint(pt[apun+7]+1,pt[apun+1]), cvPoint(pt[apun+9],pt[apun+3]), CV_RGB(255,255,255), 2, 8, 0 );
            cvRectangle( pImg, cvPoint(pt[apun+6]  ,pt[apun+2]), cvPoint(pt[apun+7],pt[apun+4]), CV_RGB(255,255,255), 2, 8, 0 );
            cvRectangle( pImg, cvPoint(pt[apun+7]+1,pt[apun+3]), cvPoint(pt[apun+8],pt[apun+4]), CV_RGB(255,255,255), 2, 8, 0 );
            *detecte=1;

        }


    if((found_d>0) && (ambos==1)) { // SOLO EL OJO DERECHO
        ancho1  = final_der.x + final_der.width;
        cuarto  = cvRound( (double)(final_der.width/5));
        pt[apun+0] = final_der.y - cuarto;
        pt[apun+1] = final_der.y - cuarto;
        pt[apun+2] = final_der.y + final_der.height; // + tercio;
        pt[apun+3] = final_der.y + final_der.height; // + tercio;
        pt[apun+4] = final_der.y + (final_der.width*2);
        pt[apun+5] = final_der.x - final_der.width;
        pt[apun+6] = final_der.x - cvRound((double)(final_der.width * 0.66));
        pt[apun+7] = final_der.x;
        pt[apun+8] = ancho1;
        pt[apun+9] = ancho1 + cuarto;

        pt[apun+0] = (pt[apun+0] > 0) ? pt[apun+0]:0;
        pt[apun+1] = (pt[apun+1] > 0) ? pt[apun+1]:0;
        pt[apun+5] = (pt[apun+5] > 0) ? pt[apun+5]:0;
        pt[apun+9] = (pt[apun+9] < pImg->width)  ? pt[apun+9] : pImg->width -1;
        pt[apun+4] = (pt[apun+4] < pImg->height) ? pt[apun+4] : pImg->height-1;

        cvRectangle( pImg, cvPoint(pt[apun+5]  ,pt[apun+0]), cvPoint(pt[apun+7],pt[apun+2]), CV_RGB(255,0,255), 2, 8, 0 );
        cvRectangle( pImg, cvPoint(pt[apun+7]+1,pt[apun+1]), cvPoint(pt[apun+9],pt[apun+3]), CV_RGB(255,0,255), 2, 8, 0 );
        cvRectangle( pImg, cvPoint(pt[apun+6]  ,pt[apun+2]), cvPoint(pt[apun+7],pt[apun+4]), CV_RGB(255,0,255), 2, 8, 0 );
        cvRectangle( pImg, cvPoint(pt[apun+7]+1,pt[apun+3]), cvPoint(pt[apun+8],pt[apun+4]), CV_RGB(255,0,255), 2, 8, 0 );
        *detecte=1;

    }
    //cvShowImage("eyes",pImg);
    //cvWaitKey();
    return pImg;
}


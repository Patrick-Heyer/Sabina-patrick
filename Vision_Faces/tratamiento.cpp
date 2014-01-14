#include "cv.h"
#include "highgui.h"
#define SIZE 4

IplImage* mejora_imagen(IplImage* image) {
    IplImage *dst_image,*nueva,*adj_img;
    dst_image= cvCreateImage(cvSize(image->width,image->height),8,1);
    adj_img= cvCreateImage(cvSize(image->width,image->height),8,1);
    nueva = cvCreateImage(cvSize(SIZE,SIZE),8,1);
    int div1=(int)(image->width/SIZE);
    int div2=(int)(image->height/SIZE);
    int ox,oy,dx,dy,mean,i,j;
    unsigned char* data = (unsigned char *)(nueva->imageData);
    int step = nueva->widthStep;
    int channel = nueva->nChannels;
    ox=oy=dx=dy=mean=0;
    int max=0;
    double minVal,maxVal;
    CvPoint minLoc, maxLoc;

    for(i=0; i<SIZE; i++) {
        for(j=0; j<SIZE; j++) {
            ox = j*div1;
            oy = i*div2;
            dx = (j+1)*div1;
            dy = (i+1)*div2;
            cvSetImageROI(image,cvRect(ox,oy,dx,dy));
            CvScalar prome = cvAvg( image,0);
            cvResetImageROI(image);
            data[i * step + j * channel] = cvRound(prome.val[0]);
        }
    }
    cvResize(nueva,dst_image,CV_INTER_LINEAR );
    cvMinMaxLoc(dst_image,&minVal,&maxVal,&minLoc,&maxLoc,0);
    cvSubRS( dst_image, cvScalar(maxVal), adj_img ,0 ); //C(I)=S-A(I) if mask(I)!=0
    cvAdd(image,adj_img,dst_image,0);
    return dst_image;
}
void histogramEqualize2(const IplImage * const pSource, IplImage * pTarget) {
    assert(pSource!=NULL);
    assert(pTarget!=NULL);
    assert(pSource->nChannels ==1);
    assert(pTarget->nChannels ==1);
    assert(pSource->width == pTarget->width);
    assert(pSource->height == pTarget->height);
    CvHistogram *hist;
    uchar lut[256];
    double lut1[256];
    CvMat* lut_mat;
    int hist_size = 256;
    float range_0[]= {0,256};
    float* ranges[] = { range_0 };

    int high=0;
    int low =0;
    float hist_value = 0.0;

    hist = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
    lut_mat = cvCreateMatHeader( 1, 256, CV_8UC1 );
    cvSetData( lut_mat, lut, 0 );
    //cvCalcHist( &pSource, hist, 0, NULL );
    cvCalcHist( const_cast<IplImage**>(&pSource), hist, 0, NULL );
    lut1[0] = 0;
    for(int index = 0; index != hist_size; ++index) {
        hist_value = cvQueryHistValue_1D(hist,index);
        lut1[index+1]= lut1[index] + hist_value;
    }
    float scale_factor;
    scale_factor = 255.0f / (float) (pSource->width * pSource->height);
    for (int index=0; index!=256; ++index) {
        lut[index]= (unsigned char)((float)lut1[index]*scale_factor);
    }
    cvLUT( pSource, pTarget, lut_mat );
    cvReleaseMat( &lut_mat);
    cvReleaseHist(&hist);
}

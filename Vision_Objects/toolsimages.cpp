#include "toolsimages.h"

ToolsImages::ToolsImages()
{
}

void ToolsImages::getKeyPoints(IplImage *imgRGB, CvSeq** imageKeyPoint, CvSeq** ImageDescriptors){

    //OBTANIN KEYPOINTS
       CvMemStorage* memoryBlock = cvCreateMemStorage();
       CvSURFParams params = cvSURFParams(500, 0);
 //product principal
       IplImage* src=imgRGB;
       IplImage* dst = cvCreateImage( cvSize( src->width, src->height ), IPL_DEPTH_8U, 1 );
        cvCvtColor(src,dst, CV_RGB2GRAY);
         cvExtractSURF(dst,0,imageKeyPoint,ImageDescriptors,memoryBlock,params);
         cvReleaseImage(&dst);

}

void ToolsImages::getKeyPoints(IplImage *imgRGB, IplImage *msk, CvSeq ** imageKeyPoint, CvSeq ** ImageDescriptors){
    //OBTANIN KEYPOINTS
       CvMemStorage* memoryBlock = cvCreateMemStorage();
       CvSURFParams params = cvSURFParams(500, 0);
 //product principal
       IplImage* src=imgRGB;
       IplImage* dst = cvCreateImage( cvSize( src->width, src->height ), IPL_DEPTH_8U, 1 );
        cvCvtColor(src,dst, CV_RGB2GRAY);
         cvExtractSURF(dst,msk,imageKeyPoint,ImageDescriptors,memoryBlock,params);
         cvReleaseImage(&dst);
//         qDebug()<<"num KEY "<<(*imageKeyPoint)->total;


}



IplImage* ToolsImages::EscalarImagen(IplImage *img, int w, int h){

    IplImage* dts=cvCreateImage( cvSize(w,h), img->depth, img->nChannels );
    cvResize(img,dts);
   return dts;
}


IplImage* ToolsImages::getSubImage(IplImage *img, CvPoint p1,int width, int height){
         IplImage* subImg=cvCreateImage(cvSize(width,height),img->depth,img->nChannels);

        CvRect roiRect = cvRect(p1.x, p1.y, width,height);

        cvSetImageROI(img, roiRect);
        cvCopy(img, subImg, NULL);
        cvResetImageROI(img);

    return subImg;


    }

IplImage* ToolsImages::getRGBNormalized(IplImage *imgrgb){

    IplImage* redchannel = cvCreateImage(cvGetSize(imgrgb), 8, 1);
    IplImage* greenchannel = cvCreateImage(cvGetSize(imgrgb), 8, 1);
    IplImage* bluechannel = cvCreateImage(cvGetSize(imgrgb), 8, 1);

        IplImage* redavg = cvCreateImage(cvGetSize(imgrgb), 8, 1);
        IplImage* greenavg = cvCreateImage(cvGetSize(imgrgb), 8, 1);
        IplImage* blueavg= cvCreateImage(cvGetSize(imgrgb), 8, 1);

        IplImage* imgavg = cvCreateImage(cvGetSize(imgrgb), 8, 3);
        cvSplit(imgrgb, bluechannel, greenchannel, redchannel, NULL);
        for(int x=0;x<imgrgb->width;x++)
            {
                for(int y=0;y<imgrgb->height;y++)
                {

                            int redValue = cvGetReal2D(redchannel, y, x);
                              int greenValue = cvGetReal2D(greenchannel, y, x);
                              int blueValue = cvGetReal2D(bluechannel, y, x);

                              double sum = redValue+greenValue+blueValue;

                              cvSetReal2D(redavg, y, x, redValue/sum*255);
                              cvSetReal2D(greenavg, y, x, greenValue/sum*255);
                              cvSetReal2D(blueavg, y, x, blueValue/sum*255);
                }
                }
         cvMerge(blueavg, greenavg, redavg, NULL, imgavg);
         cvReleaseImage(&redchannel);
          cvReleaseImage(&greenchannel);
          cvReleaseImage(&bluechannel);
          cvReleaseImage(&redavg);
          cvReleaseImage(&greenavg);
          cvReleaseImage(&blueavg);

          return imgavg;

}

IplImage* ToolsImages::getRGBNormalizedIntensity(IplImage *imgrgb){

    IplImage* redchannel = cvCreateImage(cvGetSize(imgrgb), 8, 1);
    IplImage* greenchannel = cvCreateImage(cvGetSize(imgrgb), 8, 1);
    IplImage* bluechannel = cvCreateImage(cvGetSize(imgrgb), 8, 1);

        IplImage* redavg = cvCreateImage(cvGetSize(imgrgb), 8, 1);
        IplImage* greenavg = cvCreateImage(cvGetSize(imgrgb), 8, 1);
        IplImage* blueavg= cvCreateImage(cvGetSize(imgrgb), 8, 1);

        IplImage* imgavg = cvCreateImage(cvGetSize(imgrgb), 8, 3);
        cvSplit(imgrgb, bluechannel, greenchannel, redchannel, NULL);
        for(int x=0;x<imgrgb->width;x++)
            {
                for(int y=0;y<imgrgb->height;y++)
                {

                            int redValue = cvGetReal2D(redchannel, y, x);
                              int greenValue = cvGetReal2D(greenchannel, y, x);
                              int blueValue = cvGetReal2D(bluechannel, y, x);
                              int grayValue= (0.299*redValue + 0.587*greenValue + 0.114*blueValue);

                              double sum = redValue+greenValue+blueValue;

                              cvSetReal2D(redavg, y, x, redValue/sum*255);
                              cvSetReal2D(greenavg, y, x, greenValue/sum*255);
                              cvSetReal2D(blueavg, y, x, grayValue);
                }
                }
         cvMerge(blueavg, greenavg, redavg, NULL, imgavg);
         cvReleaseImage(&redchannel);
          cvReleaseImage(&greenchannel);
          cvReleaseImage(&bluechannel);
          cvReleaseImage(&redavg);
          cvReleaseImage(&greenavg);
          cvReleaseImage(&blueavg);

          return imgavg;

}


IplImage* ToolsImages::getc1c2c3(IplImage *imgrgb){

    IplImage* redchannel = cvCreateImage(cvGetSize(imgrgb), 8, 1);
    IplImage* greenchannel = cvCreateImage(cvGetSize(imgrgb), 8, 1);
    IplImage* bluechannel = cvCreateImage(cvGetSize(imgrgb), 8, 1);

        IplImage* redavg = cvCreateImage(cvGetSize(imgrgb), 8, 1);
        IplImage* greenavg = cvCreateImage(cvGetSize(imgrgb), 8, 1);
        IplImage* blueavg= cvCreateImage(cvGetSize(imgrgb), 8, 1);

        IplImage* imgavg = cvCreateImage(cvGetSize(imgrgb), 8, 3);
        cvSplit(imgrgb, bluechannel, greenchannel, redchannel, NULL);
        for(int x=0;x<imgrgb->width;x++)
            {
                for(int y=0;y<imgrgb->height;y++)
                {

                            int redValue = cvGetReal2D(redchannel, y, x);
                              int greenValue = cvGetReal2D(greenchannel, y, x);
                              int blueValue = cvGetReal2D(bluechannel, y, x);

                              double sum = redValue+greenValue+blueValue;

                              cvSetReal2D(redavg, y, x, atan(redValue/max(greenValue,blueValue))*255);
                              cvSetReal2D(greenavg, y, x, atan(greenValue/max(redValue,blueValue))*255);
                              cvSetReal2D(blueavg, y, x, atan(blueValue/max(redValue,greenValue))*255);
                }
                }
         cvMerge(blueavg, greenavg, redavg, NULL, imgavg);
         cvReleaseImage(&redchannel);
          cvReleaseImage(&greenchannel);
          cvReleaseImage(&bluechannel);
          cvReleaseImage(&redavg);
          cvReleaseImage(&greenavg);
          cvReleaseImage(&blueavg);

          return imgavg;

}

IplImage* ToolsImages::getl1l2l3(IplImage *imgrgb){

    IplImage* redchannel = cvCreateImage(cvGetSize(imgrgb), 8, 1);
    IplImage* greenchannel = cvCreateImage(cvGetSize(imgrgb), 8, 1);
    IplImage* bluechannel = cvCreateImage(cvGetSize(imgrgb), 8, 1);

        IplImage* redavg = cvCreateImage(cvGetSize(imgrgb), 8, 1);
        IplImage* greenavg = cvCreateImage(cvGetSize(imgrgb), 8, 1);
        IplImage* blueavg= cvCreateImage(cvGetSize(imgrgb), 8, 1);

        IplImage* imgavg = cvCreateImage(cvGetSize(imgrgb), 8, 3);
        cvSplit(imgrgb, bluechannel, greenchannel, redchannel, NULL);
        for(int x=0;x<imgrgb->width;x++)
            {
                for(int y=0;y<imgrgb->height;y++)
                {

                            int redValue = cvGetReal2D(redchannel, y, x);
                              int greenValue = cvGetReal2D(greenchannel, y, x);
                              int blueValue = cvGetReal2D(bluechannel, y, x);

                              double sum = (redValue-greenValue)*(redValue-greenValue)+(redValue-blueValue)*(redValue-blueValue)
                                      +(greenValue-blueValue)*(greenValue-blueValue);

                              cvSetReal2D(redavg, y, x, (redValue-greenValue)*(redValue-greenValue)/sum*255);
                              cvSetReal2D(greenavg, y, x, (redValue-blueValue)*(redValue-blueValue)/sum*255);
                              cvSetReal2D(blueavg, y, x, (greenValue-blueValue)*(greenValue-blueValue)/sum*255);
                }
                }
         cvMerge(blueavg, greenavg, redavg, NULL, imgavg);
         cvReleaseImage(&redchannel);
          cvReleaseImage(&greenchannel);
          cvReleaseImage(&bluechannel);
          cvReleaseImage(&redavg);
          cvReleaseImage(&greenavg);
          cvReleaseImage(&blueavg);

          return imgavg;

}


double ToolsImages::max(double a, double b){
    if(a>=b)
        return a;
    else
        return b;
}



double ToolsImages::meanImg1canal(IplImage *img1, IplImage *msk){

    int M = img1->width;
    int N = img1->height;

  double sum;
    int contador1=0;
    for( int i=0; i<N; ++i )
    {

       for( int j=0; j<M; ++j )
       {
           uchar *ptr_msk = (uchar*) (msk->imageData + i*msk->widthStep);
           if(int(ptr_msk[j]) > 245){
        contador1++;
           uchar *ptr1 = (uchar*) (img1->imageData + i*img1->widthStep);

                       double r1=double(ptr1[3*j]);

                sum+=r1;


           }

       }
    }



    return sum/double(contador1);
    }

IplImage* ToolsImages::join2Images(IplImage *img1, IplImage *img2){
    int h=max(img1->height, img2->height);
    IplImage* correspond = cvCreateImage( cvSize(img1->width+img2->width+10, h), 8, 3 );
    cvSetImageROI( correspond, cvRect( 0, 0, img1->width, img1->height) );
    cvCopy( img1, correspond );
    cvSetImageROI( correspond, cvRect( img1->width+5,0,  img2->width, img2->height ) );
    cvCopy( img2, correspond );
    cvResetImageROI( correspond );

return correspond;


}

IplImage* ToolsImages::join2Imagesmsk1(IplImage *img1, IplImage *msk1, IplImage *img2){
    int h=max(img1->height, img2->height);
    IplImage* correspond = cvCreateImage( cvSize(img1->width+img2->width+10, h), 8, 3 );
    cvSetImageROI( correspond, cvRect( 0, 0, img1->width, img1->height) );
    cvCopy( img1, correspond,msk1 );
    cvSetImageROI( correspond, cvRect( img1->width+5,0,  img2->width, img2->height ) );
    cvCopy( img2, correspond );
    cvResetImageROI( correspond );

return correspond;


}

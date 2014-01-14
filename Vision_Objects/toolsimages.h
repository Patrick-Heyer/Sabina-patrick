#ifndef TOOLSIMAGES_H
#define TOOLSIMAGES_H
#include <cv.h>

class ToolsImages
{
public:
    ToolsImages();
    void getKeyPoints(IplImage* imgRGB,CvSeq** imageKeyPoint, CvSeq** ImageDescriptors);
    void getKeyPoints(IplImage* imgRGB,IplImage*msk,CvSeq**  imageKeyPoint, CvSeq** ImageDescriptors);

    IplImage* EscalarImagen(IplImage* img, int w, int h);

    IplImage* EscalarImagenChannel1(IplImage* img, int w, int h);
    IplImage* getSubImage(IplImage *img, CvPoint p1,int width, int height);
    IplImage* getRGBNormalized(IplImage* imgrgb);
    IplImage* getRGBNormalizedIntensity(IplImage* imgrgb);
    IplImage* getc1c2c3(IplImage* imgrgb);
    IplImage* getl1l2l3(IplImage* imgrgb);
    double max(double a, double b);

    double meanImg1canal(IplImage* img, IplImage* msk);

    IplImage* join2Images(IplImage *img1, IplImage *img2);
    IplImage* join2Imagesmsk1(IplImage *img1,IplImage *msk1, IplImage *img2);


};

#endif // TOOLSIMAGES_H

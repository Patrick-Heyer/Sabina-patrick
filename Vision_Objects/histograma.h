#ifndef HISTOGRAMA_H
#define HISTOGRAMA_H
#include "toolsimages.h"
#include <opencv/cv.h>
#include <highgui.h>

class Histograma
{
public:
    Histograma() {}
    void CalcularHistogramaHSV(IplImage *img1,  cv::Mat &histograma, int nh, int ns, int nv);


    void CalcularHistogramaHSV(IplImage *img1,  IplImage *msk,  cv::Mat &histograma, int nh, int ns, int nv);
    void CalcularHistogramasHSV(IplImage *img1,  IplImage *msk,  cv::Mat* &histograma, int numeroHistogramasx, int numeroHistogramasy,int nh, int ns, int nv);

    void CalcularHistogramaConjuntoHSV(IplImage *img1, IplImage *img2, cv::Mat &histograma, int nh, int ns, int nv);
    void CalcularHistogramaConjunto3Canal(IplImage *img1, IplImage *img2, cv::Mat &histograma, int nh, int ns, int nv);

    void CalcularHistograma3Canal(IplImage *img1,  cv::Mat &histograma, int n1, int n2, int n3);
    void CalcularHistograma3Canal(IplImage *img1, IplImage *msk, cv::Mat &histograma, int n1, int n2, int n3);
    void CalcularHistogramas3Canal(IplImage *img1,  IplImage *msk,  cv::Mat* &histograma, int numeroHistogramasx, int numeroHistogramasy,int nh, int ns, int nv);

    void CalculeMA_HSV(IplImage *img1,IplImage *msk,  cv::Mat &Ma_h,cv::Mat &Ma_s,cv::Mat &Ma_v, int nh, int ns, int nv);
    void CalculeMA_HSV(IplImage *img1, IplImage *msk, cv::Mat &Ma, int nh, int ns, int nv);
    void CalculeMA_RGB(IplImage *img1,IplImage *msk,  cv::Mat &Ma_r,cv::Mat &Ma_g,cv::Mat &Ma_b, int nr, int ng, int nb);
    void CalculeMA_RGB(IplImage *img1, IplImage *msk, cv::Mat &Ma, int nr, int ng, int nb);

    void CalculeMA_RGB8conect(IplImage *img1, IplImage *msk, cv::Mat &Ma, int nr, int ng, int nb);

    IplImage* CalculeIMGlabelsHSV(IplImage *img1,  int nr, int ng, int nb);
    IplImage* CalculeIMGlabels(IplImage *img1,  int nr, int ng, int nb);
};

#endif // HISTOGRAMA_H

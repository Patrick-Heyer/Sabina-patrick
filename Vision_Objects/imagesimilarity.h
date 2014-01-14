#ifndef IMAGESIMILARITY_H
#define IMAGESIMILARITY_H
#include "histograma.h"
#include "matchingsurf.h"

//#include <QDebug>

class ImageSimilarity
{


public:
    /*si las imagenes son de diferente tamaño solo calcula las medidas no puntuales */
    ImageSimilarity();

    /*valores 1 por default*/
    /*    double v_HSVhistograma;
        double v_combination;
        double v_sadHSV;
        double v_sadScilab;
        double v_keyPoints;
        double v_nMi;
        double v_correlation;
    */


    double keyPoints(std::vector<int>& indices1, std::vector<int>& indices2,CvSeq* imageDescriptors1, CvSeq* imageDescriptors2);

    /*Para los siguientes métodos es necesario que las imagenes sean del mismo tamaño,
    es decir tener un registro de imagenes*/

    double combination(IplImage* img1, IplImage* img2);
    double sadHSV(IplImage* img1, IplImage* img2,IplImage* msk);
    double sadHSV(IplImage* img1, IplImage* img2);
    double sadScilab(IplImage* img1, IplImage* img2);
    double sad3canal(IplImage* img1, IplImage* img2,IplImage* msk);

    double sadCorrelationl(IplImage* img1, IplImage* img2,IplImage* msk, double k);



    double correlation(IplImage* img1, IplImage* img2);
    double Porcent(IplImage* img1, IplImage* img2, IplImage* msk);
    double nMi(IplImage *img1, IplImage *img2,cv::Mat& histograma1, cv::Mat& histograma2,  int nh, int ns, int nv);

    double DiferencesMA(cv::Mat& MA1, cv::Mat& MA2);
    double DiferencesMA(cv::Mat*& MA1, cv::Mat*& MA2, int total);

    /*****métodos utilizados para markovito*******/
    double HSVhistograma(cv::Mat &histogram1, cv::Mat &histogram2);
    double HSVhistograma(cv::Mat* &histograms1, cv::Mat* &histograms2, int total);

    double DiferenciasCuadrado3Canal(IplImage *img1, IplImage *img2, IplImage *msk, bool spaceColorC1c2c3);
    double correlationMSK3Canal(IplImage* img1, IplImage* img2, IplImage* msk, bool spaceColorC1c2c3);
    double SpacialMSK3Canal(IplImage* img1, IplImage* img2, IplImage* msk, double k, bool spaceColorC1c2c3);//pte

    //para un canal de etiquetas..
    double DiferenciasCuadrado1Canal(IplImage *img1, IplImage *img2, IplImage *msk, int maxvalue,bool spaceColorC1c2c3);
    double correlationMSK1Canal(IplImage* img1, IplImage* img2, IplImage* msk,int maxvalue,bool spaceColorC1c2c3);
    double spacialMSK1Canal(IplImage* img1, IplImage* img2, IplImage* msk,double k,int maxvalue,bool spaceColorC1c2c3);//pte



private:




};

#endif // IMAGESIMILARITY_H

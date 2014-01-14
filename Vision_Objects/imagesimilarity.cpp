#include "imagesimilarity.h"


ImageSimilarity::ImageSimilarity()
{
}


double ImageSimilarity::DiferencesMA(cv::Mat &MA1, cv::Mat &MA2) {
    int total=MA1.rows;
    double val=0;
    for(int i=0; i<total; i++)
        for(int j=0; j<total; j++)
//            val+=(MA1.at<float>(i,j)-MA2.at<float>(i,j))*(MA1.at<float>(i,j)-MA2.at<float>(i,j));
            val+=sqrt(MA1.at<float>(i,j)*MA2.at<float>(i,j));
    //  val+=abs(MA1.at<float>(i,j)-MA2.at<float>(i,j));
    return val;///double(total*total);
}
double ImageSimilarity::DiferencesMA(cv::Mat *&MA1, cv::Mat *&MA2, int total) {
    double val=0;
    for(int i=0; i<total; i++)
        val+=DiferencesMA(MA1[i],MA2[i]);

    return val/double(total);

}



double ImageSimilarity::combination(IplImage *img1, IplImage *img2) {
    return 1;
}

double ImageSimilarity::Porcent(IplImage *img1, IplImage *img2, IplImage *msk) {
    double costo=0;
    int h=img1->height;
    int w=img2->width;

    double normalizador=0;
    double contador=0;
    double aux=1/sqrt(5);
    double pi=M_PI;
    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++) {
            uchar *ptrmsk = (uchar*) (msk->imageData + i*msk->widthStep);

            if(int(ptrmsk[j])>245) {
                uchar *ptr1 = (uchar*) (img1->imageData + i*img1->widthStep);
                uchar *ptr2 = (uchar*) (img2->imageData + i*img2->widthStep);


                double h1=2*double(ptr1[3*j]);
                double h2=2*double(ptr2[3*j]);
                double s1=double(ptr1[3*j+1])/255.0;
                double s2=double(ptr2[3*j+1])/255.0;
                double v1=double(ptr1[3*j+2])/255.0;
                double v2=double(ptr2[3*j+2])/255.0;


                costo=aux*sqrt((v1-v2)*(v1-v2)+(s1*cos(h1*pi/180)-s2*cos(h2*pi/180))*(s1*cos(h1*pi/180)-s2*cos(h2*pi/180))+(s1*sin(h1*pi/180)-s2*sin(h2*pi/180))*(s1*sin(h1*pi/180)-s2*sin(h2*pi/180)));

                if(costo<0.2)
                    contador++;
                normalizador++;
            }
        }





    return double(contador)/double(normalizador);

}



double ImageSimilarity::sad3canal(IplImage *img1, IplImage *img2,  IplImage* msk) {

    double costo=0;
    int h=img1->height;
    int w=img1->width;

    double normalizador=0.0;
    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++) {
            uchar *ptrmsk = (uchar*) (msk->imageData + i*msk->widthStep);

            if(int(ptrmsk[j])>245) {

                uchar *ptr1 = (uchar*) (img1->imageData + i*img1->widthStep);
                uchar *ptr2 = (uchar*) (img2->imageData + i*img2->widthStep);


                double h1=double(ptr1[3*j])/255;
                double h2=double(ptr2[3*j])/255;
                double s1=double(ptr1[3*j+1])/255;
                double s2=double(ptr2[3*j+1])/255;
                double v1=double(ptr1[3*j+2])/255;
                double v2=double(ptr2[3*j+2])/255;


                //    if(v1>0.10 && v2>0.10 && s1>0.1 && s2>0.1){
                //   costo+=aux*sqrt((v1-v2)*(v1-v2)+(s1*cos(h1*pi/180)-s2*cos(h2*pi/180))*(s1*cos(h1*pi/180)-s2*cos(h2*pi/180))+(s1*sin(h1*pi/180)-s2*sin(h2*pi/180))*(s1*sin(h1*pi/180)-s2*sin(h2*pi/180)));
                costo+=sqrt((v1-v2)*(v1-v2)+(s1-s2)*(s1-s2)+(h1-h2)*(h1-h2));
                normalizador+=1.0;
                //}
            }
        }








    if (normalizador==0)
        normalizador=1.0;
    return 1-costo/normalizador;
}


double ImageSimilarity::sadHSV(IplImage *img1, IplImage *img2,  IplImage* msk) {

    double costo=0;
    int h=img1->height;
    int w=img1->width;

    double normalizador=0.0;
    double aux=1/sqrt(5);
    double pi=M_PI;
    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++) {
            uchar *ptrmsk = (uchar*) (msk->imageData + i*msk->widthStep);

            if(int(ptrmsk[j])>245) {

                uchar *ptr1 = (uchar*) (img1->imageData + i*img1->widthStep);
                uchar *ptr2 = (uchar*) (img2->imageData + i*img2->widthStep);


                double h1=2*double(ptr1[3*j]);
                double h2=2*double(ptr2[3*j]);
                double s1=double(ptr1[3*j+1])/255.0;
                double s2=double(ptr2[3*j+1])/255.0;
                double v1=double(ptr1[3*j+2])/255.0;
                double v2=double(ptr2[3*j+2])/255.0;


                //    if(v1>0.10 && v2>0.10 && s1>0.1 && s2>0.1){
                costo+=aux*sqrt((v1-v2)*(v1-v2)+(s1*cos(h1*pi/180)-s2*cos(h2*pi/180))*(s1*cos(h1*pi/180)-s2*cos(h2*pi/180))+(s1*sin(h1*pi/180)-s2*sin(h2*pi/180))*(s1*sin(h1*pi/180)-s2*sin(h2*pi/180)));
//            costo+=sqrt((v1-v2)*(v1-v2)+(s1-s2)*(s1-s2)+(h1-h2)*(h1-h2));
                normalizador+=1.0;
                //}
            }
        }








    if (normalizador==0)
        normalizador=1.0;
    return 1-costo/normalizador;
}


double ImageSimilarity::sadHSV(IplImage *img1, IplImage *img2) {

    double costo=0;
    int h=img1->height;
    int w=img1->width;

    double normalizador=0.0;
    double aux=1/sqrt(5);
    double pi=M_PI;
    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++) {

            uchar *ptr1 = (uchar*) (img1->imageData + i*img1->widthStep);
            uchar *ptr2 = (uchar*) (img2->imageData + i*img2->widthStep);


            double h1=2*double(ptr1[3*j]);
            double h2=2*double(ptr2[3*j]);
            double s1=double(ptr1[3*j+1])/255.0;
            double s2=double(ptr2[3*j+1])/255.0;
            double v1=double(ptr1[3*j+2])/255.0;
            double v2=double(ptr2[3*j+2])/255.0;


            if(v1>0.10 && v2>0.10 && s1>0.1 && s2>0.1) {
                costo+=aux*sqrt((v1-v2)*(v1-v2)+(s1*cos(h1*pi/180)-s2*cos(h2*pi/180))*(s1*cos(h1*pi/180)-s2*cos(h2*pi/180))+(s1*sin(h1*pi/180)-s2*sin(h2*pi/180))*(s1*sin(h1*pi/180)-s2*sin(h2*pi/180)));
//            costo+=sqrt((v1-v2)*(v1-v2)+(s1-s2)*(s1-s2)+(h1-h2)*(h1-h2));
                normalizador+=1.0;
            }
        }
    //verificar si hay mucha información que no fue tratada  hacer el proceso sin restricciones de color
    if(normalizador<0.05*w*h) {
        normalizador=0;
        costo=0;
        for(int i=0; i<h; i++)
            for(int j=0; j<w; j++) {

                uchar *ptr1 = (uchar*) (img1->imageData + i*img1->widthStep);
                uchar *ptr2 = (uchar*) (img2->imageData + i*img2->widthStep);


                double h1=2*double(ptr1[3*j]);
                double h2=2*double(ptr2[3*j]);
                double s1=double(ptr1[3*j+1])/255.0;
                double s2=double(ptr2[3*j+1])/255.0;
                double v1=double(ptr1[3*j+2])/255.0;
                double v2=double(ptr2[3*j+2])/255.0;

                costo+=aux*sqrt((v1-v2)*(v1-v2)+(s1*cos(h1*pi/180)-s2*cos(h2*pi/180))*(s1*cos(h1*pi/180)-s2*cos(h2*pi/180))+(s1*sin(h1*pi/180)-s2*sin(h2*pi/180))*(s1*sin(h1*pi/180)-s2*sin(h2*pi/180)));
                normalizador+=1.0;

            }



    }



    if (normalizador==0)
        normalizador=1.0;
    return 1-costo/normalizador;
}


double ImageSimilarity::sadScilab(IplImage *img1, IplImage *img2) {

    IplImage* img_1=cvCreateImage(cvSize(img1->width, img1->height),IPL_DEPTH_8U,3);
    IplImage* img_2=cvCreateImage(cvSize(img2->width, img2->height),IPL_DEPTH_8U,3);

    cvCvtColor(img1,img_1,CV_RGB2Lab);
    cvCvtColor(img2,img_2,CV_RGB2Lab);

    Histograma hist;
    cv::Mat histogram1;
    cv::Mat histogram2;

    int c=6;
    hist.CalcularHistograma3Canal(img_1,histogram1,c,c,c);
    hist.CalcularHistograma3Canal(img_2,histogram2,c,c,c);


    double dif=0;
    int a=c*c*c;
    for(int i=0; i<a; i++)
        dif+=sqrt(histogram1.at<float>(i,0)*histogram2.at<float>(i,0));

    return dif;
}


double ImageSimilarity::keyPoints(vector<int>& indices1, vector<int>& indices2, CvSeq* imageDescriptors1, CvSeq* imageDescriptors2) {
    int n_keyPoints_min=10;
    MatchingSURF ms;

    int total=indices1.size();
//qDebug()<<total;
    int descriptorsCount = (int)(imageDescriptors1->elem_size/sizeof(float));



    if (total<n_keyPoints_min)
        return 0;
    else {
        double costo=0;
        for(int i=0; i<total; i++) {
            const float* image1Descriptor =  (const float*) cvGetSeqElem(imageDescriptors1, indices1.at(i));
            const float* image2Descriptor =  (const float*) cvGetSeqElem(imageDescriptors2, indices2.at(i));
            costo+=ms.compareSURFDescriptors(image1Descriptor,image2Descriptor,descriptorsCount);
        }



        return 1-costo/total;//temporal menos.
    }

}


double ImageSimilarity::nMi(IplImage *img1, IplImage *img2,cv::Mat& histograma1, cv::Mat& histograma2,  int nh, int ns, int nv) {

    //crear histograma Conjunto
    cv::Mat histconjunto;
    Histograma histo;
    histo.CalcularHistogramaConjuntoHSV(img1,img2,histconjunto,nh,ns,nv);

    int a=nh*ns*nv;
    double H1=0;
    double H2=0;
    double H12=0;

    double epsilon=0.000001;
    for(int i=0; i<a; i++) {
        float v=histograma1.at<float>(i,0);
        float v2=histograma2.at<float>(i,0);

        if(v>epsilon)
            H1+=v*log(v);
        if(v2>epsilon)
            H2+=v2*log(v2);
        for (int j=0; j<a; j++)
        {
            double v12=histconjunto.at<float>(i,j);
            if(v12>epsilon) {
                H12+=v12*log(v12);
            }
        }
    }
    H1=-H1;
    H2=-H2;
    H12=-H12;

    double NMI=(H1+H2)/H12;
    return (2-2/NMI);
}

double ImageSimilarity::correlation(IplImage *img1, IplImage *img2) {

    double corr_r,corr_g, corr_b;

    int M = img1->width;
    int N = img1->height;

    CvScalar img1_avg = cvAvg( img1, NULL );
    CvScalar img2_avg = cvAvg( img2, NULL );

    double sum_img1_img2R = 0;
    double sum_img1_2R = 0;
    double sum_img2_2R = 0;

    double sum_img1_img2G = 0;
    double sum_img1_2G = 0;
    double sum_img2_2G = 0;

    double sum_img1_img2B = 0;
    double sum_img1_2B = 0;
    double sum_img2_2B = 0;


    for( int i=0; i<N; ++i )
    {

        for( int j=0; j<M; ++j )
        {
            uchar *ptr1 = (uchar*) (img1->imageData + i*img1->widthStep);
            uchar *ptr2 = (uchar*) (img2->imageData + i*img2->widthStep);


            double r1=double(ptr1[3*j]);
            double r2=double(ptr2[3*j]);
            double g1=double(ptr1[3*j+1]);
            double g2=double(ptr2[3*j+1]);
            double b1=double(ptr1[3*j+2]);
            double b2=double(ptr2[3*j+2]);


            sum_img1_img2R   +=  (r1-img1_avg.val[0])*(r2-img2_avg.val[0]);
            sum_img1_2R      +=  (r1-img1_avg.val[0])*(r1-img1_avg.val[0]);
            sum_img2_2R      +=  (r2-img2_avg.val[0])*(r2-img2_avg.val[0]);


            sum_img1_img2G   +=  (g1-img1_avg.val[1])*(g2-img2_avg.val[1]);
            sum_img1_2G      +=  (g1-img1_avg.val[1])*(g1-img1_avg.val[1]);
            sum_img2_2G      +=  (g2-img2_avg.val[1])*(g2-img2_avg.val[1]);


            sum_img1_img2B   +=  (b1-img1_avg.val[2])*(b2-img2_avg.val[2]);
            sum_img1_2B      +=  (b1-img1_avg.val[2])*(b1-img1_avg.val[2]);
            sum_img2_2B      +=  (b2-img2_avg.val[2])*(b2-img2_avg.val[2]);

        }
    }

    corr_r = sum_img1_img2R/sqrt(sum_img1_2R*sum_img2_2R);
    corr_g = sum_img1_img2G/sqrt(sum_img1_2G*sum_img2_2G);
    corr_b = sum_img1_img2B/sqrt(sum_img1_2B*sum_img2_2B);

    return (((corr_r+corr_g+corr_b)/3.0)+1)/2;


}



/************************************************************************/


//Compara dos histogramas//
double ImageSimilarity::HSVhistograma(cv::Mat &histogram1, cv::Mat &histogram2) {
    double dif=0;
    int a=histogram1.rows;
    for(int i=0; i<a; i++)
        //  dif+=(histogram1.at<float>(i,0)-histogram2.at<float>(i,0))*(histogram1.at<float>(i,0)-histogram2.at<float>(i,0));
        dif+=sqrt(histogram1.at<float>(i,0)*histogram2.at<float>(i,0));
    return dif;
}

//compara %total% numero de histogramas y si mas del 60 % son muy parecido al  0.3 es valido.
double ImageSimilarity::HSVhistograma(cv::Mat *&histograms1, cv::Mat *&histograms2, int total) {
    int a=histograms1[0].rows;
    double* val=new double[total];
    bool* valores_buenos=new bool[total];
    int contador_buenos=0;

    for(int k=0; k<total; k++) {
        double dif=0;
        for(int i=0; i<a; i++)
            //    dif+=(histogram1.at<float>(i,0)-histogram2.at<float>(i,0))*(histogram1.at<float>(i,0)-histogram2.at<float>(i,0));
            dif+=sqrt(histograms1[k].at<float>(i,0)*histograms2[k].at<float>(i,0));
        if(dif>0.3) {
            val[k]=dif;
            valores_buenos[k]=true;
            contador_buenos++;
        }
        else {
            valores_buenos[k]=false;
            val[k]=0;
        }

    }

    double aport=1.0/double(contador_buenos);

    if(contador_buenos>total*0.6) {
        double finalval=0;
        for(int k=0; k<total; k++)
            if(valores_buenos[k])
                finalval+=aport*val[k];
        delete[] val;
        delete[] valores_buenos;
        return finalval;
    }
    else
        return 0;
}

// Método de diferencias para tres canales: c1c2c3 o HSV   /////VALIDAR MAS ESTE METODO
double ImageSimilarity::DiferenciasCuadrado3Canal(IplImage *img1, IplImage *img2, IplImage *msk, bool spaceColorC1c2c3) {
    double corr_r,corr_g, corr_b;
    assert(img1->width==img2->width && img2->width==msk->width && img1->height==img2->height && img2->height==msk->height);
    int M = img1->width;
    int N = img1->height;

//CvScalar img1_avg = cvAvg( img1, msk );

    double sum_img1_img2R = 0;
    double sum_img1_2R = 0;

    double sum_img1_img2G = 0;
    double sum_img1_2G = 0;

    double sum_img1_img2B = 0;
    double sum_img1_2B = 0;

    double m1r=0;
    /*
    if(spaceColorC1c2c3)
        m1r=img1_avg.val[0]/255;
    else
        m1r=2*img1_avg.val[0]/360;

    double m1g=img1_avg.val[1]/255;
    double m1b=img1_avg.val[2]/255;
    */

    int contador1=0;
    for( int i=0; i<N; ++i )
    {

        for( int j=0; j<M; ++j )
        {
            uchar *ptr_msk = (uchar*) (msk->imageData + i*msk->widthStep);
            if(int(ptr_msk[j]) > 245) {
                contador1++;
                uchar *ptr1 = (uchar*) (img1->imageData + i*img1->widthStep);
                uchar *ptr2 = (uchar*) (img2->imageData + i*img2->widthStep);

                double r1=0;
                double g1=0;
                double b1=0;
                double r2=0;
                double g2=0;
                double b2=0;

                if(spaceColorC1c2c3) {
                    r1=double(ptr1[3*j])/255;
                    r2=double(ptr2[3*j])/255;
                    g1=double(ptr1[3*j+1])/255;
                    g2=double(ptr2[3*j+1])/255;
                    b1=double(ptr1[3*j+2])/255;
                    b2=double(ptr2[3*j+2])/255;

                }
                else {
                    r1=double(2*ptr1[3*j])/360;
                    r2=double(2*ptr2[3*j])/360;
                    g1=double(ptr1[3*j+1])/255;
                    g2=double(ptr2[3*j+1])/255;
                    b1=double(ptr1[3*j+2])/255;
                    b2=double(ptr2[3*j+2])/255;


                }


                sum_img1_img2R   +=  (r1-r2)*(r1-r2);

                sum_img1_img2G   +=  (g1-g2)*(g1-g2);


                sum_img1_img2B   +=  (b1-b2)*(b1-b2);
            }

        }
    }

    sum_img1_img2R/=double(contador1);
    sum_img1_2R/=double(contador1);

    sum_img1_img2G/=double(contador1);
    sum_img1_2G/=double(contador1);

    sum_img1_img2B/=double(contador1);
    sum_img1_2B/=double(contador1);


    corr_r = sum_img1_img2R;
    corr_g = sum_img1_img2G;
    corr_b = sum_img1_img2B;


    /*
    corr_r = sum_img1_img2R/sum_img1_2R;
    corr_g = sum_img1_img2G/sum_img1_2G;
    corr_b = sum_img1_img2B/sum_img1_2B;
    */
//qDebug()<<corr_r;

    return 1-((corr_r+corr_g+corr_b)/3.0);
}

// Método de Correlation para tres canales: c1c2c3 o HSV
double ImageSimilarity::correlationMSK3Canal(IplImage *img1, IplImage *img2, IplImage *msk, bool spaceColorC1c2c3) {
    double corr_r,corr_g, corr_b;
    assert(img1->width==img2->width && img2->width==msk->width && img1->height==img2->height && img2->height==msk->height);
    int M = img1->width;
    int N = img1->height;

    CvScalar img1_avg = cvAvg( img1, msk );
    CvScalar img2_avg = cvAvg( img2, msk );

    double sum_img1_img2R = 0;
    double sum_img1_2R = 0;
    double sum_img2_2R = 0;

    double sum_img1_img2G = 0;
    double sum_img1_2G = 0;
    double sum_img2_2G = 0;

    double sum_img1_img2B = 0;
    double sum_img1_2B = 0;
    double sum_img2_2B = 0;

    double m1r=0;
    double m2r=0;

    if(spaceColorC1c2c3) {

        m1r=img1_avg.val[0]/255;
        m2r=img2_avg.val[0]/255;
    }
    else {
        m1r=2*img1_avg.val[0]/360;
        m2r=2*img2_avg.val[0]/360;

    }

    double m1g=img1_avg.val[1]/255;
    double m1b=img1_avg.val[2]/255;

    double m2g=img2_avg.val[1]/255;
    double m2b=img2_avg.val[2]/255;


    int contador1=0;
    for( int i=0; i<N; ++i )
    {

        for( int j=0; j<M; ++j )
        {
            uchar *ptr_msk = (uchar*) (msk->imageData + i*msk->widthStep);
            if(int(ptr_msk[j]) > 245) {
                contador1++;
                uchar *ptr1 = (uchar*) (img1->imageData + i*img1->widthStep);
                uchar *ptr2 = (uchar*) (img2->imageData + i*img2->widthStep);
                double r1=0;
                double r2=0;
                if(spaceColorC1c2c3) {
                    r1=double(ptr1[3*j])/255;
                    r2=double(ptr2[3*j])/255;
                }
                else
                {
                    r1=double(ptr1[3*j])/180;
                    r2=double(ptr2[3*j])/180;
                }

                double g1=double(ptr1[3*j+1])/255;
                double g2=double(ptr2[3*j+1])/255;
                double b1=double(ptr1[3*j+2])/255;
                double b2=double(ptr2[3*j+2])/255;


                sum_img1_img2R   +=  (r1-m1r)*(r2-m2r);
                sum_img1_2R      +=  (r1-m1r)*(r1-m1r);
                sum_img2_2R      +=  (r2-m2r)*(r2-m2r);


                sum_img1_img2G   +=  (g1-m1g)*(g2-m2g);
                sum_img1_2G      +=  (g1-m1g)*(g1-m1g);
                sum_img2_2G      +=  (g2-m2g)*(g2-m2g);


                sum_img1_img2B   +=  (b1-m1b)*(b2-m2b);
                sum_img1_2B      +=  (b1-m1b)*(b1-m1b);
                sum_img2_2B      +=  (b2-m2b)*(b2-m2b);
            }

        }
    }

    corr_r = sum_img1_img2R/sqrt(sum_img1_2R*sum_img2_2R);
    corr_g = sum_img1_img2G/sqrt(sum_img1_2G*sum_img2_2G);
    corr_b = sum_img1_img2B/sqrt(sum_img1_2B*sum_img2_2B);

    return (((corr_r+corr_g+corr_b)/3.0)+1)/2;
}




// Método de diferencias para 1 canal cuatificado: c1c2c3 o HSV   /////VALIDAR MAS ESTE METODO
double ImageSimilarity::DiferenciasCuadrado1Canal(IplImage *img1, IplImage *img2, IplImage *msk, int maxvalue, bool spaceColorC1c2c3)
{
    double corr_r,corr_g, corr_b;
    assert(img1->width==img2->width && img2->width==msk->width && img1->height==img2->height && img2->height==msk->height);
    int M = img1->width;
    int N = img1->height;

    CvScalar img1_avg = cvAvg( img1, msk );

    double sum_img1_img2R = 0;
    double sum_img1_2R = 0;

    double m1r=0;

//m1r=ToolsImages().meanImg1canal(img1, msk)/double(maxvalue);

    m1r=img1_avg.val[0]/double(maxvalue);

    int contador1=0;
    for( int i=0; i<N; ++i )
    {

        for( int j=0; j<M; ++j )
        {
            uchar *ptr_msk = (uchar*) (msk->imageData + i*msk->widthStep);
            if(int(ptr_msk[j]) > 245) {
                contador1++;
                uchar *ptr1 = (uchar*) (img1->imageData + i*img1->widthStep);
                uchar *ptr2 = (uchar*) (img2->imageData + i*img2->widthStep);

                double r1=0;
                double r2=0;
                r1=double(ptr1[3*j])/double(maxvalue);
                r2=double(ptr2[3*j])/double(maxvalue);


                sum_img1_img2R   +=  (r1-r2)*(r1-r2);
                sum_img1_2R      +=  (r1-m1r)*(r1-m1r);

            }

        }
    }

    sum_img1_img2R/=double(contador1);
    sum_img1_2R/=double(contador1);



    corr_r = sum_img1_img2R/sum_img1_2R;
    return 1-corr_r;
}


// Método de correlation para 1 canal cuantificado: c1c2c3 o HSV
double ImageSimilarity::correlationMSK1Canal(IplImage *img1, IplImage *img2, IplImage *msk, int maxvalue, bool spaceColorC1c2c3) {
    double corr_r;
    assert(img1->width==img2->width && img2->width==msk->width && img1->height==img2->height && img2->height==msk->height);
    int M = img1->width;
    int N = img1->height;

    CvScalar img1_avg = cvAvg( img1, msk );
    CvScalar img2_avg = cvAvg( img2, msk );

    double sum_img1_img2R = 0;
    double sum_img1_2R = 0;
    double sum_img2_2R = 0;

    double m1r=0;
    double m2r=0;



    m1r=img1_avg.val[0]/double(maxvalue);
    m2r=img2_avg.val[0]/double(maxvalue);

    /*double aux=ToolsImages().meanImg1canal(img1, msk);
    m1r=aux/double(maxvalue);
    m2r=ToolsImages().meanImg1canal(img2, msk)/double(maxvalue);
    */


    int contador1=0;
    for( int i=0; i<N; ++i )
    {

        for( int j=0; j<M; ++j )
        {
            uchar *ptr_msk = (uchar*) (msk->imageData + i*msk->widthStep);
            if(int(ptr_msk[j]) > 245) {
                contador1++;
                uchar *ptr1 = (uchar*) (img1->imageData + i*img1->widthStep);
                uchar *ptr2 = (uchar*) (img2->imageData + i*img2->widthStep);


                double r1=double(ptr1[3*j])/double(maxvalue);
                double r2=double(ptr2[3*j])/double(maxvalue);


                sum_img1_img2R   +=  (r1-m1r)*(r2-m2r);
                sum_img1_2R      +=  (r1-m1r)*(r1-m1r);
                sum_img2_2R      +=  (r2-m2r)*(r2-m2r);


            }

        }
    }

    corr_r = sum_img1_img2R/sqrt(sum_img1_2R*sum_img2_2R);

    return ((corr_r)+1)/2;
}









//faltan
double ImageSimilarity::spacialMSK1Canal(IplImage *img1, IplImage *img2, IplImage *msk, double k, int maxvalue, bool spaceColorC1c2c3) {
    double corr=this->correlationMSK1Canal(img1,img2,msk,maxvalue,spaceColorC1c2c3);
    double dif=1-this->DiferenciasCuadrado1Canal(img1,img2,msk,maxvalue,spaceColorC1c2c3);

//qDebug()<<corr<<"   "<<dif;
    return corr/(1+k*dif);
}

double ImageSimilarity::SpacialMSK3Canal(IplImage *img1, IplImage *img2, IplImage *msk, double k, bool spaceColorC1c2c3) {
    double corr=this->correlationMSK3Canal(img1,img2,msk,spaceColorC1c2c3);
    double dif=1-this->DiferenciasCuadrado3Canal(img1,img2,msk,spaceColorC1c2c3);


    return corr/(1+k*dif);
}

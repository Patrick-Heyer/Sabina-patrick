#include "histograma.h"

/* Calcula el histograma en canales HSV discretizados a hn*hs*nv colores*/
void Histograma::CalcularHistogramaHSV(IplImage *image, cv::Mat &histogram, int nh, int ns, int nv) {

    histogram=cv::Mat::zeros(nh*ns*nv,1,CV_32SC1);

    int w=image->width;
    int h=image->height;

    //recorrer la imagen

    double aux=0;
    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++) {
            uchar *ptr = (uchar*) (image->imageData + i*image->widthStep);

            int h1=(ptr[3*j])/(180/double(nh));
            int s1=(ptr[3*j+1])/(256/double(ns));
            int v1=(ptr[3*j+2])/(256/double(nv));
            int pos=h1+nh*s1+nh*ns*v1;
            histogram.at<float>(pos,0)+=1;
            aux+=1.0;

        }

    //verificar si hay mucha información que no fue tratada  hacer el proceso sin restricciones de color


    //normalized
//    double aux=w*h;
    if(aux==0)
        aux=1;
    for (int i=0; i<nh*ns*nv; i++)
        histogram.at<float>(i,0)/=aux;


}

/*Calcula los histogramas en canales HSV sobre una imagen sobre una respectiva mascara */
void Histograma::CalcularHistogramaHSV(IplImage *image, IplImage *msk, cv::Mat &histogram, int nh, int ns, int nv) {

    histogram=cv::Mat::zeros(nh*ns*nv,1,CV_32SC1);

    int w=image->width;
    int h=image->height;

    //recorrer la imagen

    double aux=0;
    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++) {

            uchar *ptrMSK = (uchar*) (msk->imageData + i*msk->widthStep);

            if(int(ptrMSK[j])>245) {
                uchar *ptr = (uchar*) (image->imageData + i*image->widthStep);

                int h1=(ptr[3*j])/(180/double(nh));
                int s1=(ptr[3*j+1])/(256/double(ns));
                int v1=(ptr[3*j+2])/(256/double(nv));
                int pos=h1+nh*s1+nh*ns*v1;
                histogram.at<float>(pos,0)+=1;
                aux+=1.0;
            }
        }

    //verificar si hay mucha información que no fue tratada  hacer el proceso sin restricciones de color


    //normalized
//    double aux=w*h;
    if(aux==0)
        aux=1;
    for (int i=0; i<nh*ns*nv; i++)
        histogram.at<float>(i,0)/=aux;


}

/*Divide la imagen en numero numeroHistogramasx X numeroHistogramasy y calcula lo histogramas en esa región utilizando una mascara*/
void Histograma::CalcularHistogramasHSV(IplImage *img1, IplImage *msk, cv::Mat *&histogramas, int numeroHistogramasx, int numeroHistogramasy, int nh, int ns, int nv)

{
    histogramas=new cv::Mat[numeroHistogramasx*numeroHistogramasy];

    int pixelesx=double(img1->width)/double(numeroHistogramasx);
    int pixelesy=double(img1->height)/double(numeroHistogramasy);
    ToolsImages ti;
    for(int i=0; i<numeroHistogramasy; i++) {
        int coory=pixelesy*i;
        for(int j=0; j<numeroHistogramasx; j++) {
            int coorx=j*pixelesx;
            IplImage* sub=ti.getSubImage(img1,cvPoint(coorx,coory),pixelesx,pixelesy);
            IplImage* sub_Msk=ti.getSubImage(msk,cvPoint(coorx,coory),pixelesx,pixelesy);
            this->CalcularHistogramaHSV(sub,sub_Msk,histogramas[j+i*numeroHistogramasx],nh,ns,nv);
            cvReleaseImage(&sub);
            cvReleaseImage(&sub_Msk);

        }
    }


}


/*Divide la imagen en numero numeroHistogramasx X numeroHistogramasy y calcula lo histogramas en esa región utilizando una mascara*/
void Histograma::CalcularHistogramas3Canal(IplImage *img1, IplImage *msk, cv::Mat *&histogramas, int numeroHistogramasx, int numeroHistogramasy, int nh, int ns, int nv)

{
    histogramas=new cv::Mat[numeroHistogramasx*numeroHistogramasy];

    int pixelesx=double(img1->width)/double(numeroHistogramasx);
    int pixelesy=double(img1->height)/double(numeroHistogramasy);
    ToolsImages ti;
    for(int i=0; i<numeroHistogramasy; i++) {
        int coory=pixelesy*i;
        for(int j=0; j<numeroHistogramasx; j++) {
            int coorx=j*pixelesx;
            IplImage* sub=ti.getSubImage(img1,cvPoint(coorx,coory),pixelesx,pixelesy);
            IplImage* sub_Msk=ti.getSubImage(msk,cvPoint(coorx,coory),pixelesx,pixelesy);
            this->CalcularHistograma3Canal(sub,sub_Msk,histogramas[j+i*numeroHistogramasx],nh,ns,nv);
            cvReleaseImage(&sub);
            cvReleaseImage(&sub_Msk);

        }
    }


}

/*Histograma conjunto 3 Canal*/
void Histograma::CalcularHistogramaConjunto3Canal(IplImage *img1, IplImage *img2, cv::Mat &hist, int nh, int ns, int nv) {

    //checar que las imagenes sean del mismo tamaño
    int h=img1->height;
    int w=img1->width;

    if(img2->width==w && img2->height==h) {
        hist=cv::Mat::zeros(nh*ns*nv,nh*ns*nv, CV_32SC1);


        //recorrer las imagenes
        for(int i=0; i<h; i++) {
            for(int j=0; j<w; j++) {
                uchar *ptr1 = (uchar*) (img1->imageData + i*img1->widthStep);
                uchar *ptr2 = (uchar*) (img2->imageData + i*img2->widthStep);

                //     uchar *ptr1=ptr2;
                int h1=(ptr1[3*j])/(256/double(nh));
                int h2=(ptr2[3*j])/(256/double(nh));
                int s1=(ptr1[3*j+1])/(256/double(ns));
                int s2=(ptr2[3*j+1])/(256/double(ns));
                int v1=(ptr1[3*j+2])/(256/double(nv));
                int v2=(ptr2[3*j+2])/(256/double(nv));

                int pos1=h1+nh*s1+nh*ns*v1;
                int pos2=h2+nh*s2+nh*ns*v2;

                //      if(pos1>18*3*3 || pos2>18*3*3)
                //        qDebug()<<"mayor en conjunta";

                hist.at<float>(pos1,pos2)+=1;

            }

        }

    }//fin if
    else
        hist=cv::Mat::ones(nh*ns*nv,nh*ns*nv, CV_32SC1);//??


    //normalized
    double aux=w*h;
    for (int i=0; i<nh*ns*nv; i++)
        for (int j=0; j<nh*ns*nv; j++) {
            hist.at<float>(i,j)/=aux;
        }
}

/*Histograma conjunto HSV*/
void Histograma::CalcularHistogramaConjuntoHSV(IplImage *img1, IplImage *img2, cv::Mat &hist, int nh, int ns, int nv) {

    //checar que las imagenes sean del mismo tamaño
    int h=img1->height;
    int w=img1->width;

    if(img2->width==w && img2->height==h) {
        hist=cv::Mat::zeros(nh*ns*nv,nh*ns*nv, CV_32SC1);


        //recorrer las imagenes
        for(int i=0; i<h; i++) {
            for(int j=0; j<w; j++) {
                uchar *ptr1 = (uchar*) (img1->imageData + i*img1->widthStep);
                uchar *ptr2 = (uchar*) (img2->imageData + i*img2->widthStep);

                //     uchar *ptr1=ptr2;
                int h1=(ptr1[3*j])/(180/double(nh));
                int h2=(ptr2[3*j])/(180/double(nh));
                int s1=(ptr1[3*j+1])/(256/double(ns));
                int s2=(ptr2[3*j+1])/(256/double(ns));
                int v1=(ptr1[3*j+2])/(256/double(nv));
                int v2=(ptr2[3*j+2])/(256/double(nv));

                int pos1=h1+nh*s1+nh*ns*v1;
                int pos2=h2+nh*s2+nh*ns*v2;

                //      if(pos1>18*3*3 || pos2>18*3*3)
                //        qDebug()<<"mayor en conjunta";

                hist.at<float>(pos1,pos2)+=1;

            }

        }

    }//fin if
    else
        hist=cv::Mat::ones(nh*ns*nv,nh*ns*nv, CV_32SC1);//??


    //normalized
    double aux=w*h;
    for (int i=0; i<nh*ns*nv; i++)
        for (int j=0; j<nh*ns*nv; j++) {
            hist.at<float>(i,j)/=aux;
        }
}


/*calcula un histograma discretizado en canal RGB indicando la discretización en n1, n2 y  n3*/
void Histograma::CalcularHistograma3Canal(IplImage *image, cv::Mat &histogram, int n1, int n2, int n3)
{

    histogram=cv::Mat::zeros(n1*n2*n3,1,CV_32SC1);

    int w=image->width;
    int h=image->height;

    //recorrer la imagen


    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++) {
            uchar *ptr = (uchar*) (image->imageData + i*image->widthStep);

            int a=(ptr[3*j])/(256/double(n1));
            int b=(ptr[3*j+1])/(256/double(n2));
            int c=(ptr[3*j+2])/(256/double(n3));

            int pos=a+n1*b+n1*n2*c;

            histogram.at<float>(pos,0)+=1;

        }


    //normalized
    double aux=w*h;
    for (int i=0; i<n1*n2*n3; i++)
        histogram.at<float>(i,0)/=aux;

}


void Histograma::CalcularHistograma3Canal(IplImage *image, IplImage *msk, cv::Mat &histogram, int n1, int n2, int n3) {

    histogram=cv::Mat::zeros(n1*n2*n3,1,CV_32SC1);

    int w=image->width;
    int h=image->height;

    //recorrer la imagen

    double aux=0;
    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++) {
            uchar *msk_ = (uchar*) (msk->imageData + i*msk->widthStep);

            if(msk_[j]>245) {
                uchar *ptr = (uchar*) (image->imageData + i*image->widthStep);

                int a=(ptr[3*j])/(256/double(n1));
                int b=(ptr[3*j+1])/(256/double(n2));
                int c=(ptr[3*j+2])/(256/double(n3));

                int pos=a+n1*b+n1*n2*c;

                histogram.at<float>(pos,0)+=1;
                aux++;

            }
        }


    //normalized

    for (int i=0; i<n1*n2*n3; i++)
        histogram.at<float>(i,0)/=aux;

}

/*Calcula la matriz adyacente de la imagen discretizada en nh, ns,nv */
void Histograma::CalculeMA_HSV(IplImage *img1, IplImage *msk, cv::Mat &Ma, int nh, int ns, int nv) {
    Ma=cv::Mat::zeros(nh*ns*nv,nh*ns*nv,CV_32SC1);
    cv::Mat  MaAux=cv::Mat::zeros(nh*ns*nv,nh*ns*nv,CV_32SC1);

    int w=img1->width;
    int h=img1->height;
    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++) {

            uchar *msk_ = (uchar*) (msk->imageData + i*msk->widthStep);

            if(msk_[j]>245) {
                uchar *img_ = (uchar*) (img1->imageData + i*img1->widthStep);
                int a=(img_[3*j])/(180/double(nh));
                int b=(img_[3*j+1])/(256/double(ns));
                int c=(img_[3*j+2])/(256/double(nv));
                int pos1=a+nh*b+ns*nv*c;

                if(i>=1) {
                    uchar *img_2 = (uchar*) (img1->imageData + (i-1)*img1->widthStep);
                    uchar *msk_2 = (uchar*) (msk->imageData + (i-1)*msk->widthStep);
                    if(msk_2[j]>245) {
                        int a_=(img_2[3*(j)])/(180/double(nh));
                        int b_=(img_2[3*(j)+1])/(256/double(ns));
                        int c_=(img_2[3*(j)+2])/(256/double(nv));
                        int pos2=a_+nh*b_+ns*nv*c_;
                        MaAux.at<float>(pos1, pos2)+=1;
                    }
                }//end if

                if(j>=1) {
                    uchar *img_2 = (uchar*) (img1->imageData + (i)*img1->widthStep);
                    uchar *msk_2 = (uchar*) (msk->imageData + (i)*msk->widthStep);
                    if(msk_2[j-1]>245) {
                        int a_=(img_2[3*(j-1)])/(180/double(nh));
                        int b_=(img_2[3*(j-1)+1])/(256/double(ns));
                        int c_=(img_2[3*(j-1)+2])/(256/double(nv));
                        int pos2=a_+nh*b_+ns*nv*c_;
                        MaAux.at<float>(pos1, pos2)+=1;
                    }

                }//end if

            }//end if

        }//end for



//normalizar
    int count=0;
    for (int i=0; i<nh*ns*nv; i++)
        for (int j=0; j<nh*ns*nv; j++) {
            Ma.at<float>(i,j)=MaAux.at<float>(i,j)+MaAux.at<float>(j,i);
            count+=Ma.at<float>(i,j);
        }

    for (int i=0; i<nh*ns*nv; i++)
        for (int j=0; j<nh*ns*nv; j++) {
            Ma.at<float>(i,j)/=double(count);
        }
    MaAux.release();


    /*
    cv::Mat* Maas=new cv::Mat(nh*ns*nv,nh*ns*nv,CV_8U);

    for (int i=0;i<nh*ns*nv;i++)
        for (int j=0;j<nh*ns*nv;j++){
            Maas->at<int>(i,j)= Ma.at<float>(i,j)*255;
        }

    */
//    cvShowImage("this window",Maas);
    //  cvWaitKey(0);
}//end function


/*Calcula la matriz adyacente de la imagen discretizada en nt, ng,nb */
void Histograma::CalculeMA_RGB(IplImage *img1, IplImage *msk, cv::Mat &Ma, int nr, int ng, int nb) {
    Ma=cv::Mat::zeros(nr*ng*nb,nr*ng*nb,CV_32SC1);
    cv::Mat  MaAux=cv::Mat::zeros(nr*ng*nb,nr*ng*nb,CV_32SC1);

    int w=img1->width;
    int h=img1->height;
    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++) {

            uchar *msk_ = (uchar*) (msk->imageData + i*msk->widthStep);

            if(msk_[j]>245) {
                uchar *img_ = (uchar*) (img1->imageData + i*img1->widthStep);
                int a=(img_[3*j])/(256/double(nr));
                int b=(img_[3*j+1])/(256/double(ng));
                int c=(img_[3*j+2])/(256/double(nb));
                int pos1=a+nr*b+ng*nb*c;

                if(i>=1) {
                    uchar *img_2 = (uchar*) (img1->imageData + (i-1)*img1->widthStep);
                    uchar *msk_2 = (uchar*) (msk->imageData + (i-1)*msk->widthStep);
                    if(msk_2[j]>245) {
                        int a_=(img_2[3*(j)])/(256/double(nr));
                        int b_=(img_2[3*(j)+1])/(256/double(ng));
                        int c_=(img_2[3*(j)+2])/(256/double(nb));
                        int pos2=a_+nr*b_+ng*nb*c_;
                        MaAux.at<float>(pos1, pos2)+=1;
                    }
                }//end if

                if(j>=1) {
                    uchar *img_2 = (uchar*) (img1->imageData + (i)*img1->widthStep);
                    uchar *msk_2 = (uchar*) (msk->imageData + (i)*msk->widthStep);
                    if(msk_2[j-1]>245) {
                        int a_=(img_2[3*(j-1)])/(256/double(nr));
                        int b_=(img_2[3*(j-1)+1])/(256/double(ng));
                        int c_=(img_2[3*(j-1)+2])/(256/double(nb));
                        int pos2=a_+nr*b_+ng*nb*c_;
                        MaAux.at<float>(pos1, pos2)+=1;
                    }

                }//end if

            }//end if

        }//end for



//normalizar
    int count=0;
    for (int i=0; i<nr*ng*nb; i++)
        for (int j=0; j<nr*ng*nb; j++) {
            Ma.at<float>(i,j)=MaAux.at<float>(i,j)+MaAux.at<float>(j,i);
            count+=Ma.at<float>(i,j);
        }

    for (int i=0; i<nr*ng*nb; i++)
        for (int j=0; j<nr*ng*nb; j++) {
            Ma.at<float>(i,j)/=double(count);
        }
    MaAux.release();

//    cvShowImage("this window",&Ma);
    //  cvWaitKey(0);
}//end function




/*Calcula 3 matriz adyacentes de los canales RGB de la imagen discretizada en nr, ng,nb */
void Histograma::CalculeMA_RGB(IplImage *img1, IplImage *msk, cv::Mat &Ma_r, cv::Mat &Ma_g, cv::Mat &Ma_b, int nr, int ng, int nb) {
    Ma_r=cv::Mat::zeros(nr,nr,CV_32SC1);
    cv::Mat  Mar_Aux=cv::Mat::zeros(nr,nr,CV_32SC1);
    Ma_g=cv::Mat::zeros(ng,ng,CV_32SC1);
    cv::Mat  Mag_Aux=cv::Mat::zeros(ng,ng,CV_32SC1);
    Ma_b=cv::Mat::zeros(nb,nb,CV_32SC1);
    cv::Mat  Mab_Aux=cv::Mat::zeros(nb,nb,CV_32SC1);

    int w=img1->width;
    int h=img1->height;
    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++) {

            uchar *msk_ = (uchar*) (msk->imageData + i*msk->widthStep);

            if(msk_[j]>245) {
                uchar *img_ = (uchar*) (img1->imageData + i*img1->widthStep);
                int a=(img_[3*j])/(256/double(nr));
                int b=(img_[3*j+1])/(256/double(ng));
                int c=(img_[3*j+2])/(256/double(nb));

                if(i>=1) {
                    uchar *img_2 = (uchar*) (img1->imageData + (i-1)*img1->widthStep);
                    uchar *msk_2 = (uchar*) (msk->imageData + (i-1)*msk->widthStep);
                    if(msk_2[j]>245) {
                        int a_=(img_2[3*(j)])/(256/double(nr));
                        int b_=(img_2[3*(j)+1])/(256/double(ng));
                        int c_=(img_2[3*(j)+2])/(256/double(nb));
                        Mar_Aux.at<float>(a, a_)+=1;
                        Mag_Aux.at<float>(b, b_)+=1;
                        Mab_Aux.at<float>(c, c_)+=1;

                    }
                }//end if

                if(j>=1) {
                    uchar *img_2 = (uchar*) (img1->imageData + (i)*img1->widthStep);
                    uchar *msk_2 = (uchar*) (msk->imageData + (i)*msk->widthStep);
                    if(msk_2[j-1]>245) {
                        int a_=(img_2[3*(j-1)])/(256/double(nr));
                        int b_=(img_2[3*(j-1)+1])/(256/double(ng));
                        int c_=(img_2[3*(j-1)+2])/(256/double(nb));
                        Mar_Aux.at<float>(a, a_)+=1;
                        Mag_Aux.at<float>(b, b_)+=1;
                        Mab_Aux.at<float>(c, c_)+=1;
                    }

                }//end if

            }//end if

        }//end for



//normalizar
    int count=0;
    for (int i=0; i<nr; i++)
        for (int j=0; j<nr; j++) {
            Ma_r.at<float>(i,j)=Mar_Aux.at<float>(i,j)+Mar_Aux.at<float>(j,i);
            count+=Ma_r.at<float>(i,j);
        }

    for (int i=0; i<nr; i++)
        for (int j=0; j<nr; j++) {
            Ma_r.at<float>(i,j)/=double(count);
        }

    count=0;
    for (int i=0; i<nb; i++)
        for (int j=0; j<nb; j++) {
            Ma_b.at<float>(i,j)=Mab_Aux.at<float>(i,j)+Mab_Aux.at<float>(j,i);
            count+=Ma_b.at<float>(i,j);
        }

    for (int i=0; i<nb; i++)
        for (int j=0; j<nb; j++) {
            Ma_b.at<float>(i,j)/=double(count);
        }

    count=0;
    for (int i=0; i<ng; i++)
        for (int j=0; j<ng; j++) {
            Ma_g.at<float>(i,j)=Mag_Aux.at<float>(i,j)+Mag_Aux.at<float>(j,i);
            count+=Ma_g.at<float>(i,j);
        }

    for (int i=0; i<ng; i++)
        for (int j=0; j<ng; j++) {
            Ma_g.at<float>(i,j)/=double(count);
        }

    Mar_Aux.release();
    Mag_Aux.release();
    Mab_Aux.release();
}//end function




/*Calcula 3 matriz adyacentes de los canales HSV de la imagen discretizada en nh, ns,nv */
void Histograma::CalculeMA_HSV(IplImage *img1, IplImage *msk, cv::Mat &Ma_h, cv::Mat &Ma_s, cv::Mat &Ma_v, int nh, int ns, int nv) {
    Ma_h=cv::Mat::zeros(nh,nh,CV_32SC1);
    cv::Mat  Mah_Aux=cv::Mat::zeros(nh,nh,CV_32SC1);
    Ma_s=cv::Mat::zeros(ns,ns,CV_32SC1);
    cv::Mat  Mas_Aux=cv::Mat::zeros(ns,ns,CV_32SC1);
    Ma_v=cv::Mat::zeros(nv,nv,CV_32SC1);
    cv::Mat  Mav_Aux=cv::Mat::zeros(nv,nv,CV_32SC1);

    int w=img1->width;
    int h=img1->height;
    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++) {

            uchar *msk_ = (uchar*) (msk->imageData + i*msk->widthStep);

            if(msk_[j]>245) {
                uchar *img_ = (uchar*) (img1->imageData + i*img1->widthStep);
                int a=(img_[3*j])/(180/double(nh));
                int b=(img_[3*j+1])/(256/double(ns));
                int c=(img_[3*j+2])/(256/double(nv));

                if(i>=1) {
                    uchar *img_2 = (uchar*) (img1->imageData + (i-1)*img1->widthStep);
                    uchar *msk_2 = (uchar*) (msk->imageData + (i-1)*msk->widthStep);
                    if(msk_2[j]>245) {
                        int a_=(img_2[3*(j)])/(180/double(nh));
                        int b_=(img_2[3*(j)+1])/(256/double(ns));
                        int c_=(img_2[3*(j)+2])/(256/double(nv));
                        Mah_Aux.at<float>(a, a_)+=1;
                        Mas_Aux.at<float>(b, b_)+=1;
                        Mav_Aux.at<float>(c, c_)+=1;

                    }
                }//end if

                if(j>=1) {
                    uchar *img_2 = (uchar*) (img1->imageData + (i)*img1->widthStep);
                    uchar *msk_2 = (uchar*) (msk->imageData + (i)*msk->widthStep);
                    if(msk_2[j-1]>245) {
                        int a_=(img_2[3*(j-1)])/(180/double(nh));
                        int b_=(img_2[3*(j-1)+1])/(256/double(ns));
                        int c_=(img_2[3*(j-1)+2])/(256/double(nv));
                        Mah_Aux.at<float>(a, a_)+=1;
                        Mas_Aux.at<float>(b, b_)+=1;
                        Mav_Aux.at<float>(c, c_)+=1;
                    }

                }//end if

            }//end if

        }//end for



//normalizar
    int count=0;
    for (int i=0; i<nh; i++)
        for (int j=0; j<nh; j++) {
            Ma_h.at<float>(i,j)=Mah_Aux.at<float>(i,j)+Mah_Aux.at<float>(j,i);
            count+=Ma_h.at<float>(i,j);
        }

    for (int i=0; i<nh; i++)
        for (int j=0; j<nh; j++)
            Ma_h.at<float>(i,j)/=double(count);

    count=0;
    for (int i=0; i<ns; i++)
        for (int j=0; j<ns; j++) {
            Ma_s.at<float>(i,j)=Mas_Aux.at<float>(i,j)+Mas_Aux.at<float>(j,i);
            count+=Ma_s.at<float>(i,j);
        }

    for (int i=0; i<ns; i++)
        for (int j=0; j<ns; j++)
            Ma_s.at<float>(i,j)/=double(count);


    count=0;
    for (int i=0; i<nv; i++)
        for (int j=0; j<nv; j++) {
            Ma_v.at<float>(i,j)=Mav_Aux.at<float>(i,j)+Mav_Aux.at<float>(j,i);
            count+=Ma_v.at<float>(i,j);
        }

    for (int i=0; i<nv; i++)
        for (int j=0; j<nv; j++)
            Ma_v.at<float>(i,j)/=double(count);



    Mah_Aux.release();
    Mas_Aux.release();
    Mav_Aux.release();
}//end function
/*Calcula la matriz adyacente de la imagen discretizada en nt, ng,nb */
void Histograma::CalculeMA_RGB8conect(IplImage *img1, IplImage *msk, cv::Mat &Ma, int nr, int ng, int nb) {
    Ma=cv::Mat::zeros(nr*ng*nb,nr*ng*nb,CV_32SC1);

    int w=img1->width;
    int h=img1->height;
    int count=0;
    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++) {

            if(i>=1 && j>=1) {
                uchar *msk_ = (uchar*) (msk->imageData + i*msk->widthStep);

                if(msk_[j]>245) {
                    uchar *img_ = (uchar*) (img1->imageData + i*img1->widthStep);
                    int a=(img_[3*j])/(256/double(nr));
                    int b=(img_[3*j+1])/(256/double(ng));
                    int c=(img_[3*j+2])/(256/double(nb));
                    int pos1=a+nr*b+ng*nb*c;
                    for(int k=-1; k<2; k++) {
                        uchar *img_2 = (uchar*) (img1->imageData + (i+k)*img1->widthStep);
                        uchar *msk_2 = (uchar*) (msk->imageData + (i+k)*msk->widthStep);
                        for(int l=-1; l<2; l++) {
                            if(msk_2[j+l]>245) {
                                int a_=(img_2[3*(j+l)])/(256/double(nr));
                                int b_=(img_2[3*(j+l)+1])/(256/double(ng));
                                int c_=(img_2[3*(j+l)+2])/(256/double(nb));
                                int pos2=a_+nr*b_+ng*nb*c_;
                                count++;
                                Ma.at<float>(pos1, pos2)+=1;
                            }

                        }//end for
                    }//end for
                }//end if

            }//end if

        }//end for



//normalizar
    for (int i=0; i<nr*ng*nb; i++)
        for (int j=0; j<nr*ng*nb; j++)
            Ma.at<float>(i,j)/=double(count);



}//end function


//imagen de etiquets
IplImage* Histograma::CalculeIMGlabels(IplImage *img1, int nr, int ng, int nb)
{
    IplImage* labels=cvCreateImage(cvSize(img1->width, img1->height),16,1);

    int w=img1->width;
    int h=img1->height;
    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++) {


            uchar *img_ = (uchar*) (img1->imageData + i*img1->widthStep);
            uchar *labels_ = (uchar*) (labels->imageData + i*labels->widthStep);

            int a=(img_[3*j])/(256/double(nr));
            int b=(img_[3*j+1])/(256/double(ng));
            int c=(img_[3*j+2])/(256/double(nb));
            int pos1=a+nr*b+ng*nb*c;
            labels_[j]=pos1;


        }//end for

    return labels;


}
IplImage* Histograma::CalculeIMGlabelsHSV(IplImage *img1, int nr, int ng, int nb)
{
    IplImage* labels=cvCreateImage(cvSize(img1->width, img1->height),16,1);

    int w=img1->width;
    int h=img1->height;
    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++) {


            uchar *img_ = (uchar*) (img1->imageData + i*img1->widthStep);
            uchar *labels_ = (uchar*) (labels->imageData + i*labels->widthStep);

            int a=(img_[3*j])/(180/double(nr));
            int b=(img_[3*j+1])/(256/double(ng));
            int c=(img_[3*j+2])/(256/double(nb));
            int pos1=a+nr*b+ng*nb*c;
            labels_[j]=pos1;


        }//end for

    return labels;


}

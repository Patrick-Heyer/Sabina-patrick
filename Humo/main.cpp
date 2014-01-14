// g++ `pkg-config --cflags opencv` -o HUMO main.cpp `pkg-config --libs opencv`


#include "Thread.h"
#include "NetThread.h"
#include "gsNetPoll_g++.h"


#include <iostream>
#include <opencv/cv.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace std;
using namespace cv;

IplImage* skipNFrames(CvCapture* capture, int n); //Agregar función, se salta frames.
Mat DibujarCirculos(Mat IM, Mat& Mask); //Agregar función, dibuja círculos.

int main(int argc, char **argv) {
    //CvCapture* capture = cvCaptureFromFile("H1.MP4");
//     CvCapture* capture = cvCaptureFromFile("2013-04-18-184021.ogv");
    CvCapture* capture = cvCaptureFromCAM(1);

    // Definiciones
    IplImage* frame = 0; //Apuntador a frames.
    double filt[] = {1, 2, 1, 0, 0, 0, -1, -2, -1}; //Filtro en float.
    Mat filtro = Mat(3, 3, CV_64F, filt).clone(); //Guarda el filtro anterior.
    int QUITAR_RUIDO = 1; // Bandera para hacer PREPROCESO A LA IMAGEN.
    int flag1 = 0; //Bandera para saltarse el PRIMER frame.
    Mat gray, image; //Contiene la imagen en escala de grises y en colores.
    Mat Salida, FiltradoH, FiltradoV, resta_doble, Mask; //Guardan la imagen filtrada y una máscara.
    int contador = 0, contador_frames = 0; //Variables para contar.
    long Nframe = 0, ContaNeg = 0; //variable de numero de frames total. (puede omitirse)
    Mat BuferV, BuferH, BuferImg; //Buferes de imágenes del frame anterior.
    float Val, numPoints;
    int Nrows, Ncols, encontrado = 0;
    //fin Definiciones.

    if(!cvGrabFrame(capture)) {             // captura un frame
        cout<<"Could not grab a frame"<<endl; // Esto no fue un video!
        exit(0); //salida forzada.
    }
    NetThread *net = new  NetThread();
    net->SetOutputStream(stdout);
    sleep(5);
    net->OpenOutputAddress("localhost", 2080);

    do {
        //ESCOGER LO QUE CONVENGA:
        frame = skipNFrames(capture, 3); // Se salta un frame.
//       frame = cvQueryFrame(capture); //No se salta ningún frame.
        if(frame == NULL)
        {
            for(int f=0; f<=10; f++)
            {
                net->Write("FIRE");
            }
            break; //se sale si el frame es nulo.
        }
        if(Nframe >= 2000)
        {
            for(int f=0; f<=10; f++)
            {
                net->Write("FIRE");
            }
            break;
        }

        image = frame; //Le pasa la direccion del frame a imagen.

        if(QUITAR_RUIDO)
        {   //Siempre es Verdadero
            cvtColor(image, gray, CV_RGB2GRAY); //pasar a grises.
            gray.convertTo(gray,CV_64F); //pasar a double.
            gray = gray / 255.0; //redimensionar los datos al rango [0-1]
            blur(gray, Salida, Size(5,5), Point(-1, -1) ); //suaviza la imagen. Un Size grande [[QUITA RUIDO]] (Exceso de Puntos)
            filter2D(Salida, FiltradoH, -1, filtro, Point(-1,-1), 0.0, BORDER_DEFAULT ); //filtrado Horizontal
            filter2D(Salida.t(), FiltradoV, -1, filtro, Point(-1,-1), 0.0, BORDER_DEFAULT ); // Filtrado Vertical con imagen gris traspuesta.
            FiltradoV = FiltradoV.t(); //Corrige la inversión de la imagen aplicando traspuesta.
        }

        if(flag1)
        {
            resta_doble = (FiltradoV + FiltradoH) - (BuferV + BuferH); //Tecnica aplicada para detectar movimiento de tipo humo.

            Mask        = resta_doble > 0.32; // [[UMBRAL]] PARA detectar pixeles candidatos a humo
            Val   = sum(Mask)[0]/ 255.0; //Numero de puntos-Humo detectados.

            if( Val > 0)
            {
                contador++; //Se suma el contador si hubo al menos un pixel clasficado como movimiento.
                cout<<"Detectando Cambios..."<<endl;
            } else {
                //Cuando la image no tiene movimiento.
                ContaNeg++;
                cout<<"No hay cambios en la imagen"<<endl;
            }

            if(contador >= 10) //si en 25 frames de 25 hay humo... [[[PRIMER IF]]]
            {
                Mat ImgPintada = DibujarCirculos(image, Mask);
                cvNamedWindow("Deteccion de Humo", CV_WINDOW_AUTOSIZE);
                imshow("Deteccion de Humo", ImgPintada);
                cvWaitKey(50);
                cout <<"LOGRADO!!!: hay Humo, son "<<Val<<" puntos en el frame: "<< Nframe <<endl;
// 	  imshow("Deteccion de Humo", ImgPintada);
                std::stringstream path;
                path << "../data/fire/"  << "humo.png";
                imwrite(path.str(), ImgPintada);
                for(int f=0; f<=10; f++)
                {
                    net->Write("FIRE");
                }
                encontrado = 1;
                break;
                //contador = 0;
                // YO digo tambien que HABILITAR: contador_frames = 0;
// 	  if( ContaNeg < 5 ){ //Codigo para Colgar el humo más tiempo.
// 	    contador_frames--;
// 	  }
//
            }


        } else {
            //Preguntar las dimensiones de esto. (Esto SOLO se ejecuta UNA vez.
            Nrows = Salida.rows;
            Ncols = Salida.cols;
            flag1 = 1;
        }

        Salida.convertTo(BuferImg, CV_64F); //Pasar el frame actual a búferes.
        FiltradoV.convertTo(BuferV, CV_64F);
        FiltradoH.convertTo(BuferH, CV_64F);

        Nframe++;
        //Los frames en los que se detecta posible humo deben ser consecutivos,
        if (++contador_frames == 25) {
            contador_frames = 0;
            contador = 0; //Reset del contador.
            ContaNeg = 0;;
        }

    } while( frame != NULL);

    cout<<"Fin de Leer el Video"<<endl;
    cvReleaseCapture(&capture);
    cvDestroyWindow("frame");
    return encontrado;
}


IplImage* skipNFrames(CvCapture* capture, int n)
{
    for(int i = 0; i < n; ++i)
    {
        if(cvQueryFrame(capture) == NULL)
        {
            return NULL;
        }
    }

    return cvQueryFrame(capture);
}

Mat DibujarCirculos(Mat IM, Mat& Mask) {
    Mat Lienzo;
    Mat R_accel;
    Mat Mascara;
    Mat FINAL;
    Mask.convertTo(Mascara, CV_32F);
    IM.convertTo(Lienzo,CV_8UC3);
    for(int r = 0; r < Mask.rows; r++)
    {
        R_accel = Mask.row(r);
        if(sum(R_accel)[0] == 0)
        {
            continue;
        }
        for(int c = 0; c < Mask.cols; c++)
        {
            if (Mascara.at<float>(r,c) > 0)
            {
                ellipse(Lienzo,Point(c,r), Size(12, 12), 0, 0, 360, Scalar(255,255,0), 2); //Dubujar Elipse
            }// fin del IF-
        } // Fin del c-FOR
    } //fin del r-FOR
    //return Lienzo.convertTo(FINAL,CV_8UC3);
    return Lienzo;
}// fin de la Función.

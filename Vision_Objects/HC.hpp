// g++ `pkg-config --cflags opencv` -o HC HC.cpp `pkg-config --libs opencv`
#ifndef HC_HPP
#define HC_HPP
#include <iostream>
#include <opencv/cv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>


#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/nonfree/features2d.hpp>



using namespace cv;
using namespace std;

class HC
{
public:
    HC();
    HC(int Rows, int Cols);
    ~HC();
    Mat Execute(vector<KeyPoint>& KeyPuntos, Mat& Ventanas, Mat& Centros_OUT);
private:
    Mat QuitarPuntos      (Mat& CompactMatrix, int Pcol, int Prow, Mat& CompactPoints); // 5 en realidad, esta es una matriz de Clusteres.
    Mat RecaulcularScore  (Mat& ScoreElements, int Pcol, int Prow); //quita los 2 elementos, y sus valores los suma al final. (para saber cuantos elementos tiene cada cluster.
    void RecalcularMinMaxRectangle(Mat& MinWinClust, Mat& MaxWinClust, int Pcol,int Prow, float& minROW, float& minCOL, float& maxROW, float& maxCOL);
    Mat RecalcDistNewPoint(Mat& CompactMatrix, float NewProw, float NewPcol, Mat& CompactPoints, float& Infinite);
    void AddWindow        (Mat& Ventanas, int& rowini, int& rowend, int& colini, int& colend, int Puntero);
    Mat CompactVentanas   (Mat CompactV, int p);
    float UMBRAL_MATAR_HUERFANOS;
    float UMB_DIST; //Umbral: Criterio de Paro para el k-means jerarquico.
    float VistaRows;  //Promedio de Altura de las imágenes "Vista"
    float VistaCols;  //Promedio de Anchura de las imágenes "Vista"
    int ImgRows;    // Alto de la imagen.
    int ImgCols;    // Ancho de la imagen.
    float UMB_NCLUST; //Número mínimo de elementos que debe tener el cluster para ser convertido a ventana.
};

#endif

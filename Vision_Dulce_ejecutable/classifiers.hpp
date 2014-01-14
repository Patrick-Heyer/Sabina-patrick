#ifndef __MRH_HPP__
#define __MRH_HPP__

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "cvtexture_demo.hpp"


using namespace std;
using namespace cv;

class LOCAL {
public:
  LOCAL();
  //~LOCAL();
  Mat getFeaturesLocal(Mat &test);
  
  int nClassifiers;
  //int concepts;
  vector< Mat> descriptors;
  vector<double> metric;
  vector<double> probabilityLocal;
};

class GLOBAL {
public:
  GLOBAL();
  //~GLOBAL();
  vector<double> getFeaturesGlobal(Mat &img, Mat &imgGray, GLCM &glcm, int &nClassifiers);
  void color(Mat &img, vector<double> &globalFratures);
  void gabor(Mat &img, vector<double> &globalFratures);
  //void getModelsNB(string folder, int &nClassifiers);
  void getProbability(vector<double> &featuresG, int &nClassifiers);
  
  vector<double> probabilityGlobal;
  vector<Mat> TPC_HistP;
  vector<Mat> TPC_HistN;
  vector<Mat> RangoHP;
  vector<Mat> RangoHN;
};

class CLASSIFIERS {
public:
  CLASSIFIERS();
  //~CLASSIFIERS();
   void loadData(int n, int i);
   void getModelsNB(int n, int i);
   void getDescriptors(int n, int i);
   void getProbability( Mat &featuresL, vector<double> &PL);   
   vector<int> vl_ubcmatch(Mat &featuresL, Mat &descriptors);
     
   Mat TPC_HistP;
   Mat TPC_HistN;
   Mat RangoHP;
   Mat RangoHN; 
   Mat descriptors;
   double metric;
   double probG;
   double probL;

};

class OBJECT {
public:
  OBJECT();
  //~object();
  void getDescriptorsViews(int objs, int nclassifiers);
  void getParams(int n);
  void loadClassifiers(string name, int number, int &nclassifiers);
  void sendImgSize(int w, int h);
  vector<double> sendFeatures( Mat &featuresL, int &nclassifiers);
  vector<double> fusionProbabilities(vector<double> &PL);
  void sendRectangle(int existObject, int pt1x, int pt1y, int windowX, int windowY);
  void drawRectangle(Mat &image, int pt1x, int pt1y, int pt2x, int pt2y, Mat &coordinate, int concepts, string name);
  void drawRectangle(Mat &image, int pt1x, int pt1y, int pt2x, int pt2y, Mat &coordinate, int concepts, string name, Mat &h);
  void TransformarPunto(int Row, int Col, int& OutPutRow, int& OutPutCol, Mat &H);
  //void drawRectangle(Mat &image);
  
  vector<CLASSIFIERS> listClassifier;
  vector<Mat> descriptorsViews;
  int width;
  int height;
  Mat matrizFrec;
  double L;
  double E;
  double F;
  double VistaRows; // prom de rows de imgs de entrenamiento
  double VistaCols; 
  
};

class RECOGNITION {
public:
  RECOGNITION(int &n, string &name);
  RECOGNITION();
  //~RECOGNITION();  
  //vector<double> fusionProbabilities(vector<double> &PL, int &nClassifiers);
  Mat foundObjects (IplImage* img, LOCAL &local, GLOBAL &global, GLCM &glcm, int  nclassifiers, int UMB_NCLUST, float umHuerfanos, float umDist);
  void createHomografy();
  vector<int> vl_ubcmatch2(Mat &featuresL, Mat &descriptors);
  Mat hierarchicalCluster( Mat &Puntos, float UMBRAL_MATAR_HUERFANOS, float UMB_DIST, int VistaRows, int VistaCols, int ImgRows, int ImgCols, int UMB_NCLUST);
  Mat QuitarPuntos(Mat& CompactMatrix, int Pcol, int Prow, Mat &CompactPoints);
  Mat RecaulcularScore(Mat& ScoreElements, int Pcol, int Prow);
  Mat RecalcularDistancia_NewPoint(Mat& CompactMatrix, float NewProw, float NewPcol, Mat& CompactPoints, float& Infinite);
  void AddWindow(Mat& Ventanas, int& rowini, int& rowend, int& colini, int& colend, int Puntero);
  Mat CompactarVentanas(Mat CompactV, int p);
  
  int concepts;  
  vector<string> namesConcepts;
  vector<OBJECT> listObject;
  Mat H;
};


#endif
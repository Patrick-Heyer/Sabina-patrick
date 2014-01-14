#include <opencv/cv.h>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/nonfree/nonfree.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv/cxcore.hpp>
#include <stdio.h>
#include <fstream>
#include "classifiers.hpp"
#include "cvtexture_demo.hpp"


using namespace cv;

LOCAL::LOCAL(){
}

Mat LOCAL:: getFeaturesLocal(Mat &test){
  SiftDescriptorExtractor detector( 0.05, 5.0 );
  vector<KeyPoint> keypoints;
  detector.detect( test, keypoints );
   
  SiftDescriptorExtractor extractor( 3.0 );
  Mat descriptor;
  extractor.compute( test, keypoints, descriptor );
  //cout << descriptor << endl;
  return descriptor;
}


GLOBAL::GLOBAL() {  // CONSTRUCTOR
}

vector<double> GLOBAL:: getFeaturesGlobal(Mat &imgColor, Mat &imgGray, GLCM &glcm, int &nClassifiers){
  vector<double> globalFratures;
  color(imgColor, globalFratures);
  glcm.getGLCM(imgGray,globalFratures);
  gabor(imgGray,globalFratures); 
  return globalFratures;
}


void GLOBAL:: color(Mat &img, vector<double> &globalFratures ){
  //RGB
  vector<double> resVec;
  int histSize = 20; 			/// Establish the number of bins
  float range[] = { 0, 255 } ;		/// Set the ranges ( for B,G,R) )
  const float* histRange = { range };
  bool uniform = true; bool accumulate = false;
  Mat  hist, chanel, trans, hsv, cie;
  
  /// Compute the histograms:
  vector<Mat> planes; 		/// Separate the image in 3 places ( B, G and R )
  split( img, planes );
  
  //RGB ->BGR planes
  for (int i = 2 ; i >= 0; i--){
    calcHist( &planes[i], 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate );
    chanel = hist/sum(hist)[0];
    trans = chanel.t();
    trans.copyTo(resVec);
    globalFratures.insert(globalFratures.end(),resVec.begin(),resVec.end());
    //cout << infColor.size() << " " << infColor[54] <<endl;
  }
   
  //HSV
  cvtColor(img,hsv, CV_RGB2HSV);
  split( hsv, planes );
  for (int i = 0 ; i < 3; i++){
    calcHist( &planes[i], 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate );
    chanel = hist/sum(hist)[0];
    trans = chanel.t();
    trans.copyTo(resVec);
    globalFratures.insert(globalFratures.end(),resVec.begin(),resVec.end());
  }
  
  //CIELAB
  cvtColor(img,cie,CV_RGB2Lab);
  split( cie, planes );
  for (int i = 0 ; i < 3; i++){
    calcHist( &planes[i], 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate );
    chanel = hist/sum(hist)[0];
    trans = chanel.t();
    trans.copyTo(resVec);
    globalFratures.insert(globalFratures.end(),resVec.begin(),resVec.end());
  }
}

void GLOBAL:: gabor(Mat &img, vector<double> &globalFratures){
  
  Mat dst_e11_r0, dst_e11_r45, dst_e11_r90, dst_e11_r135;
  Mat dst_e25_r0, dst_e25_r45, dst_e25_r90, dst_e25_r135;

    Size Escala1(10,10); //Width y Height de Escala 11
    Size Escala2(24,24); //Width y Height de Escala 25
    double Ang0  = 0;
    double Ang45 = 45 * 3.141592 / 180.0;
    double Ang90 = 90 * 3.141592 / 180.0;
    double Ang135= 135* 3.141592 / 180.0;
    double lambda11 = 11.0 * 2.0 / 3.9;  //Es una constante asociada a la escala.
    double lambda24 = 24.0 * 2.0 / 3.05;
    double sigma11 = lambda11 * 0.8; //Aspecto dependiente de la escala.
    double sigma24 = lambda24 * 0.8;
    double gamma = 0.3; //valor constante por defecto.
    double psi = 0; //desfase por defecto.
   // el ultimo parametro puede ser CV_64F o CV_32F. El que uno guste. el de 64 incrementa la precisión.
    // Genera los Kernels del Filtro Gabor.
    Mat kernel_esc11_rot0  = cv::getGaborKernel(Escala1, sigma11, Ang0,   lambda11, gamma, psi, CV_32F );
    Mat kernel_esc11_rot45 = cv::getGaborKernel(Escala1, sigma11, Ang45,  lambda11, gamma, psi, CV_32F );
    Mat kernel_esc11_rot90 = cv::getGaborKernel(Escala1, sigma11, Ang90,  lambda11, gamma, psi, CV_32F );
    Mat kernel_esc11_rot135= cv::getGaborKernel(Escala1, sigma11, Ang135, lambda11, gamma, psi, CV_32F );

    Mat kernel_esc25_rot0  = cv::getGaborKernel(Escala2, sigma24, Ang0,   lambda24, gamma, psi, CV_32F );
    Mat kernel_esc25_rot45 = cv::getGaborKernel(Escala2, sigma24, Ang45,  lambda24, gamma, psi, CV_32F );
    Mat kernel_esc25_rot90 = cv::getGaborKernel(Escala2, sigma24, Ang90,  lambda24, gamma, psi, CV_32F );
    Mat kernel_esc25_rot135= cv::getGaborKernel(Escala2, sigma24, Ang135, lambda24, gamma, psi, CV_32F );
    //cv::getGaborKernel( Size ksize, double sigma, double theta,
    //                             double lambd, double gamma, double psi, int ktype )
    // void filter2D(Mat src, Mat dst, int ddepth, Mat kernel, Point anchor, double delta, int borderType);

    /* El -1 es para que la profundidad en bits sea la misma que la entrada. O sea, ni
     se cual es la entrada... :)
    El 0.0 es un delta, un valor que se suma a lo que salga de la convolución.
    opciones finales:

    BORDER_REPLICATE
    BORDER_CONSTANT
    BORDER_REFLECT_101
    BORDER_WARP
    BORDER_TRANSPARENT
    BORDER_DEFAULT (same as reflect)
    BORDER_ISOLATED

     */
    // Obtiene la media de cada Kernel.
    Scalar SumaE11_R0   = mean(kernel_esc11_rot0);
    Scalar SumaE11_R45  = mean(kernel_esc11_rot45);
    Scalar SumaE11_R90  = mean(kernel_esc11_rot90);
    Scalar SumaE11_R135 = mean(kernel_esc11_rot135);
    
    Scalar SumaE25_R0   = mean(kernel_esc25_rot0);
    Scalar SumaE25_R45  = mean(kernel_esc25_rot45);
    Scalar SumaE25_R90  = mean(kernel_esc25_rot90);
    Scalar SumaE25_R135 = mean(kernel_esc25_rot135);
    // Scalar Plorok = sum(kernel_esc11_rot0);
    // double Cosa = Plorok[0] / (double)(kernel_esc11_rot0.cols * kernel_esc11_rot0.rows);
    
    //Resta la media a los kernels.
    kernel_esc11_rot0   = kernel_esc11_rot0   - SumaE11_R0[0];
    kernel_esc11_rot45  = kernel_esc11_rot45  - SumaE11_R45[0];
    kernel_esc11_rot90  = kernel_esc11_rot90  - SumaE11_R90[0];
    kernel_esc11_rot135 = kernel_esc11_rot135 - SumaE11_R135[0];
    
    kernel_esc25_rot0   = kernel_esc25_rot0   - SumaE25_R0[0];
    kernel_esc25_rot45  = kernel_esc25_rot45  - SumaE25_R45[0];
    kernel_esc25_rot90  = kernel_esc25_rot90  - SumaE25_R90[0];
    kernel_esc25_rot135 = kernel_esc25_rot135 - SumaE25_R135[0];
    
    // Los suaviza, tal como se hizo en MatLab.
    kernel_esc11_rot0   = kernel_esc11_rot0   / sqrt(sum(kernel_esc11_rot0.dot (kernel_esc11_rot0))[0]);
    kernel_esc11_rot45  = kernel_esc11_rot45  / sqrt(sum(kernel_esc11_rot45.dot(kernel_esc11_rot45))[0]);
    kernel_esc11_rot90  = kernel_esc11_rot90  / sqrt(sum(kernel_esc11_rot90.dot(kernel_esc11_rot90))[0]);
    kernel_esc11_rot135 = kernel_esc11_rot135 / sqrt(sum(kernel_esc11_rot135.dot(kernel_esc11_rot135))[0]);
    
    kernel_esc25_rot0   = kernel_esc25_rot0   / sqrt(sum(kernel_esc25_rot0.dot (kernel_esc25_rot0))[0]);
    kernel_esc25_rot45  = kernel_esc25_rot45  / sqrt(sum(kernel_esc25_rot45.dot(kernel_esc25_rot45))[0]);
    kernel_esc25_rot90  = kernel_esc25_rot90  / sqrt(sum(kernel_esc25_rot90.dot(kernel_esc25_rot90))[0]);
    kernel_esc25_rot135 = kernel_esc25_rot135 / sqrt(sum(kernel_esc25_rot135.dot(kernel_esc25_rot135))[0]);
    
    // cout<<"el valor es: "<<SumaE11_R0[0]<<"y ademas nada"<< endl;
    
    // aplicando Convolución para obtener las imágenes de Salida.
    //HAGO una INVERSION de los archivos de 45 y 135 grados, porque MATLAB los maneja ALREVES.
    // En realidad, OpenCV esta cruzado. 0 y 90 sí los realiza correctamente.
    filter2D(img, dst_e11_r0,   -1, kernel_esc11_rot0,   Point(-1,-1), 0.0, BORDER_DEFAULT );
    filter2D(img, dst_e11_r135,  -1, kernel_esc11_rot45,  Point(-1,-1), 0.0, BORDER_DEFAULT );
    filter2D(img, dst_e11_r90,  -1, kernel_esc11_rot90,  Point(-1,-1), 0.0, BORDER_DEFAULT );
    filter2D(img, dst_e11_r45, -1, kernel_esc11_rot135, Point(-1,-1), 0.0, BORDER_DEFAULT );
    
    filter2D(img, dst_e25_r0,   -1, kernel_esc25_rot0,   Point(-1,-1), 0.0, BORDER_DEFAULT );
    filter2D(img, dst_e25_r135,  -1, kernel_esc25_rot45,  Point(-1,-1), 0.0, BORDER_DEFAULT );
    filter2D(img, dst_e25_r90,  -1, kernel_esc25_rot90,  Point(-1,-1), 0.0, BORDER_DEFAULT );
    filter2D(img, dst_e25_r45, -1, kernel_esc25_rot135, Point(-1,-1), 0.0, BORDER_DEFAULT );
    
    double m = mean(dst_e11_r0)[0];
    Mat d  = dst_e11_r0 - m;
    double dst = sum(d.dot(d))[0];
    globalFratures.push_back(m);
    globalFratures.push_back(dst);
    
    m = mean(dst_e11_r135)[0];
    d  = dst_e11_r0 - m;
    dst = sum(d.dot(d))[0];
    globalFratures.push_back(m);
    globalFratures.push_back(dst);
    
    m = mean(dst_e11_r90)[0];
    d  = dst_e11_r0 - m;
    dst = sum(d.dot(d))[0];
    globalFratures.push_back(m);
    globalFratures.push_back(dst);
    
    m = mean(dst_e11_r45)[0];
    d  = dst_e11_r0 - m;
    dst = sum(d.dot(d))[0];
    globalFratures.push_back(m);
    globalFratures.push_back(dst);
    
    m = mean(dst_e25_r0)[0];
    d  = dst_e11_r0 - m;
    dst = sum(d.dot(d))[0];
    globalFratures.push_back(m);
    globalFratures.push_back(dst);
    
    m = mean(dst_e11_r135)[0];
    d  = dst_e11_r0 - m;
    dst = sum(d.dot(d))[0];
    globalFratures.push_back(m);
    globalFratures.push_back(dst);
    
    m = mean(dst_e11_r45)[0];
    d  = dst_e11_r0 - m;
    dst = sum(d.dot(d))[0];
    globalFratures.push_back(m);
    globalFratures.push_back(dst);
    
    m = mean(dst_e11_r90)[0];
    d  = dst_e11_r0 - m;
    dst = sum(d.dot(d))[0];
    globalFratures.push_back(m);
    globalFratures.push_back(dst);
    
    //cout << globalFratures.size() << globalFratures[195] << endl;
	// output file
    // imwrite( "result.bmp", result );

}


RECOGNITION::RECOGNITION(){
}

RECOGNITION::RECOGNITION(int &n, string &name){
//  cout << "name " << name << endl;
  string line;
  ifstream txt;
  txt.open("objects.txt"); 

  while(!txt.eof()){
  getline(txt, line);
  if (line  == "") break;
  namesConcepts.push_back(line);
  }  
  txt.close();
  
  /////////////
  for(int i = 0; i < namesConcepts.size(); i++){
    if (name.compare((string)namesConcepts[i]) == 0){
      concepts = i + 1;
    }      
  }
  /////////////
  createHomografy();
  //concepts = namesConcepts.size();
  OBJECT objects[concepts - (concepts -1)];
    
  //for(int i = 0; i < concepts; i++){
     //cout << namesConcepts[i] << endl; 
    objects[0].loadClassifiers(name, concepts , n); // se cargan los clasificadores 
    objects[0].getDescriptorsViews(concepts, n);
    listObject.push_back(objects[0]);
  //}
}

// Mat RECOGNITION:: foundObjects (Mat &image, LOCAL &local, GLOBAL &global, GLCM &glcm, int  nclassifiers){
//   CvPoint pt1; 
//   int window = 2;
//   int slinding = 4;
//   int d = (window * slinding) - (slinding - 1);
//   int slindingX, slindingY, windowX, windowY; 
//   
//   windowX = image.cols / window;
//   windowY = image.rows / window;
//   slindingX = windowX / slinding;
//   slindingY =  windowY / slinding;
//   
//   for(int i = 0; i < concepts; i++){
//   listObject[i].sendImgSize(image.cols, image.rows);
//   }
//   
//   for(int i = 0; i < d; i++){ // slinding in x
//      pt1.x = slindingX * i;
//     for(int j = 0; j < d; j++){ // slinding in y
//      pt1.y = slindingY * j;
//       //cout << pt1.x << " " << pt1.y << " "  << windowX << " " << windowY << endl;
//       Mat testColor, testGrey;
//       testColor = Mat(image, Rect(pt1.x, pt1.y, windowX - 1, windowY  - 1));
//       cvtColor(testColor,testGrey, CV_RGB2GRAY);
//       //extraer caracteristicas
//       vector<double> featuresG = global.getFeaturesGlobal(testColor, testGrey, glcm, nclassifiers);
//       //   for(int i = 0; i < 212; i++){
//       //     cout << featuresG[i];
//       //   }
//       //   cout << endl;
//       Mat featuresL = local.getFeaturesLocal(testGrey);
//       //cout << descriptor << endl;
//       
//       for (int i = 0; i < concepts; i++){
// 	int existObject = listObject[i].sendFeatures(featuresG, featuresL, nclassifiers);
// 	listObject[i].sendRectangle(existObject, pt1.x, pt1.y, windowX - 1, windowY -1);
//       }
//       
//        imshow("ROI", testColor);
//        cvWaitKey(0); //se queda esperando a que se presione una tecla
// 
//     }
//   }
//   
//   Mat coordinate = Mat::zeros(concepts,4, CV_8UC3);
//   for(int i = 0; i < concepts; i++){  //porel el momento solo regresa las coordenadas de un ojbeto
//     listObject[i].drawRectangle(image, coordinate, i);
//   }
// 
//   return coordinate;
// }





Mat RECOGNITION:: foundObjects (IplImage* img, LOCAL &local, GLOBAL &global, GLCM &glcm, int  nclassifiers, int UMB_NCLUST, float umHuerfanos, float umDist){
  //Mat test  = cv::imread( "cap_0022.jpg", CV_LOAD_IMAGE_GRAYSCALE );
//   cout << "sali 1" << endl;
  Mat image = img;
  //cout << "rows " << image.rows << " cols"<< image.cols << endl;
     //*********************************************************************************************************************************
//   int  yesResize = 0;
//   if (yesResize == 1){
//     int factor = 4;
//     int x = image.cols / factor;
//     int y = image.rows / factor;
//     
//     Mat imgResize(y,x, CV_32F);
//     
//     resize(image, imgResize, imgResize.size(), 0, 0, INTER_LINEAR);
//     image = imgResize;
//   }
  //*********************************************************************************************************************************
   
  
  Mat gray;
  cvtColor(image, gray, CV_RGB2GRAY);
  int auxConcepts = concepts - (concepts -1);
  //cout <<  << auxConcepts << endl;
  Mat coordinate = Mat::zeros(concepts - (concepts -1),6, CV_32F);
  int imgCols = image.cols;
  int imgRows = image.rows;
  //cout << "umHuerfanos " << umHuerfanos << endl;
  //cout << "imgcol" << imgCols << " " << imgRows << endl;
  
 
  std::vector<KeyPoint> keypoints1;
  Mat descriptor1;

  cv::SiftFeatureDetector detector(0.04, 10.0);
  cv::SiftDescriptorExtractor extractor (3.0);  
  detector.detect(gray, keypoints1);  
  //cout << "keypoints1 " << keypoints1.size() << endl;  
  extractor.compute(gray, keypoints1, descriptor1);
//   cout << "sali 2" << endl;
  for (int c = 0; c < (concepts - (concepts -1)); c++){
    vector<Mat> descriptorView = listObject[c].descriptorsViews;
    vector<int> locs;
    std::vector<KeyPoint> keypoints;

    double probWindow = 0;
    int nWindow = -1;
    //cargar descriptores view
//     cout << "sali 3" << endl;
    for(int  i = 0; i < nclassifiers; i++){
      //cout << descriptor1.rows << " " << descriptorView[i].rows << endl;
      vector<int> loc =  vl_ubcmatch2(descriptor1, descriptorView[i]);
      locs.insert( locs.end(),loc.begin(), loc.end());
      //cout << " descriptor" << locs.size() << endl;
    }
    
    std::vector<int>::iterator it;
    it = std::unique (locs.begin(), locs.end());   // 10 20 30 20 10 ?  ?  ?  ?
    locs.resize( std::distance(locs.begin(),it) ); // 10 20 30 20 10

    for (it=locs.begin(); it!=locs.end(); ++it){
      keypoints.push_back(keypoints1[*it]);
    }
    Mat pts = Mat::zeros(keypoints.size(),2, CV_32F);
        //cout << "concepts " << locs.size() << endl;
    for(int i = 0; i < keypoints.size(); i++){
      pts.at<float>(i,0) = keypoints[i].pt.y;
      pts.at<float>(i,1) = keypoints[i].pt.x;
    }
//     cout << "sali 4" << endl;
    Mat windows = hierarchicalCluster(pts, umHuerfanos, umDist, listObject[c].VistaRows, listObject[c].VistaCols, imgRows, imgCols,  UMB_NCLUST);
//     cout << "sali 4.1" << endl;
    CvPoint pt1, pt2;
    int  temp = 0;
    for(int i = 0; i < windows.rows; i++){
      temp = windows.at<float>(i,0); pt1.y = temp; 
      temp = windows.at<float>(i,1); pt2.y = temp;
      temp = windows.at<float>(i,2); pt1.x = temp;
      temp = windows.at<float>(i,3); pt2.x = temp;
//       cvRectangle(src, pt1 , pt2 , CV_RGB(0,0,255), 3, 8, 0 ); //Dibujamos el rectangulo
//       cvShowImage("Ejemplo", src);
//       cvWaitKey(0); //se queda esperando a que se presione una tecla
      //////////////////////////////////////////////////////////////////
      Mat testColor, testGrey;
//       cout << "sali 4.2" << endl;
      testColor = Mat(image, Rect(pt1.x, pt1.y, pt2.x - pt1.x, pt2.y - pt1.y));
      cvtColor(testColor,testGrey, CV_RGB2GRAY);
      //imshow("ROI", testColor);
      //cvWaitKey(0); //se queda esperando a que se presione una tecla
      //extraer caracteristicas
      //vector<double> featuresG = global.getFeaturesGlobal(testColor, testGrey, glcm, nclassifiers);
      //   for(int i = 0; i < 212; i++){
      //     cout << featuresG[i];
      //   }
      //   cout << endl;
      Mat featuresL = local.getFeaturesLocal(testGrey);
      //cout << descriptor << endl;
//       cout << "sali 4.3" << endl;
      //for (int i = 0; i < concepts; i++){
	// debe devolver probabilidad
	//vector<double> existObject = listObject[c].sendFeatures(featuresG, featuresL, nclassifiers);
	vector<double> existObject = listObject[c].sendFeatures(featuresL, nclassifiers);
	if ((existObject[1] > probWindow) && (existObject[0] == 1)){
	  probWindow = existObject[1] ;
	  nWindow = i;
	}
 	//cout << "sali 4.5" << endl;
    }
    
    	//listObject[i].sendRectangle(existObject, pt1.x, pt1.y, windowX - 1, windowY -1);
    	
    	//window sacar puntos...
//     	cout << "sali 5" << endl;
	if (nWindow != -1){
	  int temp = 0;
	  temp = windows.at<float>(nWindow,0); pt1.y = temp;
	  temp = windows.at<float>(nWindow,1); pt2.y = temp;
	  temp = windows.at<float>(nWindow,2); pt1.x = temp;
	  temp = windows.at<float>(nWindow,3); pt2.x = temp;
	  //listObject[c].drawRectangle(image,pt1.x, pt1.y, pt2.x, pt2.y, coordinate, c, namesConcepts[concepts - 1], H);
	  listObject[c].drawRectangle(image,pt1.x, pt1.y, pt2.x, pt2.y, coordinate, c, namesConcepts[concepts - 1]);
	}
      //}

//   cout << "sali 6" << endl;
    
  }
  
   return coordinate;
}

void RECOGNITION::createHomografy(){
    //Formato Es: {ROW, COL, UNO}
    //Webcam
    float P11[] = {496,  550,  1}; // Left Sup
    float P12[] = {515,  1241, 1}; // Right Sup
    float P13[] = {1272, 1233, 1}; // Right Inf 
    float P14[] = {1280, 538,  1}; // Left Inf
    
    //KINECT
    float P21[] = {188, 353}; // left sup
    float P22[] = {188, 505}; // right sup
    float P23[] = {346, 509}; // right inf
    float P24[] = {344, 353}; // left inf
    
    float A1_1 = P11[0];
    float A1_2 = P11[1];
    float A1_3 = P11[2];
    float A2_4 = P11[0];
    float A2_5 = P11[1];
    float A2_6 = P11[2];
    float A1_7 = -P21[0] * P11[0];
    float A1_8 = -P21[0] * P11[1];
    float A2_7 = -P21[1] * P11[0];
    float A2_8 = -P21[1] * P11[1];
    float A1_9 = -P21[0];  
    float A2_9 = -P21[1];

    float A3_1 = P12[0];
    float A3_2 = P12[1];
    float A3_3 = P12[2];
    float A4_4 = P12[0];
    float A4_5 = P12[1];
    float A4_6 = P12[2];
    float A3_7 = -P22[0] * P12[0];
    float A3_8 = -P22[0] * P12[1];
    float A4_7 = -P22[1] * P12[0];
    float A4_8 = -P22[1] * P12[1];
    float A3_9 = -P22[0];  
    float A4_9 = -P22[1];

    float A5_1 = P13[0];
    float A5_2 = P13[1];
    float A5_3 = P13[2];
    float A6_4 = P13[0];
    float A6_5 = P13[1];
    float A6_6 = P13[2];
    float A5_7 = -P23[0] * P13[0];
    float A5_8 = -P23[0] * P13[1];
    float A6_7 = -P23[1] * P13[0];
    float A6_8 = -P23[1] * P13[1];
    float A5_9 = -P23[0];  
    float A6_9 = -P23[1];

    float A7_1 = P14[0];
    float A7_2 = P14[1];
    float A7_3 = P14[2];
    float A8_4 = P14[0];
    float A8_5 = P14[1];
    float A8_6 = P14[2];
    float A7_7 = -P24[0] * P14[0];
    float A7_8 = -P24[0] * P14[1];
    float A8_7 = -P24[1] * P14[0];
    float A8_8 = -P24[1] * P14[1];
    float A7_9 = -P24[0];  
    float A8_9 = -P24[1];
    //cout<<"NAAAAADAAAA 000000000"<<endl;
    float T[] = //8 Renglones y 9 columnas
    {A1_1, A1_2, A1_3, 0.0 , 0.0 , 0.0 , A1_7, A1_8, A1_9,
     0.0 , 0.0 , 0.0 , A2_4, A2_5, A2_6, A2_7, A2_8, A2_9,
     A3_1, A3_2, A3_3, 0.0 , 0.0 , 0.0 , A3_7, A3_8, A3_9,
     0.0 , 0.0 , 0.0 , A4_4, A4_5, A4_6, A4_7, A4_8, A4_9,
     A5_1, A5_2, A5_3, 0.0 , 0.0 , 0.0 , A5_7, A5_8, A5_9,
     0.0 , 0   , 0.0 , A6_4, A6_5, A6_6, A6_7, A6_8, A6_9,
     A7_1, A7_2, A7_3, 0.0 , 0.0 , 0.0 , A7_7, A7_8, A7_9,
     0.0 , 0.0 , 0.0 , A8_4, A8_5, A8_6, A8_7, A8_8, A8_9};
    
    
    Mat A = Mat(8,9, CV_32F, T).clone();
    Mat B = A.t() * A; //Matriz simétrica
    
    Mat V; // eigen vectores
    Mat D ;// eigen valores.
    eigen(B,D,V);
    float t_H[] = {
      V.at<float>(8,0), V.at<float>(8,1), V.at<float>(8,2),
      V.at<float>(8,3), V.at<float>(8,4), V.at<float>(8,5),
      V.at<float>(8,6), V.at<float>(8,7), V.at<float>(8,8),
    };
    H = Mat(3,3, CV_32F, t_H).clone();
    //cout<<"H tiene: "<<endl<<H<<endl; 
    
    // Al final H debe ser un DATO MIEMBRO DE LA CLASE.
    // todo LO EJECUTADO ARRIBA, SOLO DEBE EJECUTARSE UNA UNICA VEZ. y lo UNICO
    // que DEBE GUARDARSE es H.
    
    //Argumentos: RenglonDeKinect, ColumnaDeKinect, Renglon de Camarita, ColumnadeCamarita.
//     int R, C;
//     TransformarPunto(614, 752, R, C, H);
//     TransformarPunto(941, 1043, R, C, H);
//     cout<< "Salida es Row:"<< R << " y Col:"<<C <<endl;
    
    
    
  // %H Calculation,  P1 H = P2 // Solution Ax=0
  // %Solving the system   
}

void OBJECT:: TransformarPunto(int Row, int Col, int& OutPutRow, int& OutPutCol, Mat &H){
  float Punto[] = {(float)Row, (float)Col, 1.0};
  Mat P = Mat(3, 1, CV_32F, Punto).clone();
  //cout <<"P TIENE: "<<endl<<P<<endl;
  //cout <<" H " << H.rows << " " << H.cols << " P " << P.rows << " " << P.cols << endl;
  Mat NewPoint = H * P; //No necesita la traspuesta porque se declaro a P como vector columna de 3filas x 1columna.
  //cout <<" Final Tiene"<<endl<<NewPoint<<endl;
  OutPutRow = (int) (NewPoint.at<float>(0,0) / NewPoint.at<float>(2,0));
  OutPutCol = (int) (NewPoint.at<float>(1,0) / NewPoint.at<float>(2,0));
}


vector<int> RECOGNITION:: vl_ubcmatch2(Mat &featuresL, Mat &descriptors){
  double UMBRAL = 1.1; //Este es el umbral, puede ser considerado como parámetro.
  
  int N_feats = featuresL.rows; 
  int N_descripts = descriptors.rows; 
  Mat Values(N_feats, N_descripts, CV_32F);
  Mat Diff;
  
  vector<int> matching;
  double N_min, N_max, VV;
  Point Pmin, Pmax;
  int loc;
  
  for (int r = 0; r < N_feats; r++)
    for (int c = 0; c < N_descripts; c++){
      Diff = featuresL.row(r) - descriptors.row(c);
      VV = Diff.dot(Diff);
      Values.at<float>(r,c) = sqrt(VV); //Guarda TOdas las distancias en una Matriz.
    }
    float temp;
    for (int r = 0; r < N_feats; r++){ //Evalua tipo UbcMatch cada renglón.
      minMaxLoc(Values.row(r), &N_min, &N_max, &Pmin, &Pmax); //obtiene los minimos de distancias
      Values.at<float>(r, Pmin.x) = N_max + 1 ;
      temp = N_min;
      minMaxLoc(Values.row(r), &N_min, &N_max, &Pmin, &Pmax); //obtiene los minimos de nuevo
       // cout<< "publico el menor: " << temp << " y el que le sigue: "<< N_min<< endl;
      if((temp * UMBRAL) < N_min)
      {
	matching.push_back(r);
	//cout << "loc " << r << endl;
      }
    }
    return matching;
}


Mat RECOGNITION:: hierarchicalCluster( Mat &Puntos, float UMBRAL_MATAR_HUERFANOS, float UMB_DIST, int VistaRows, 
				       int VistaCols, int ImgRows, int ImgCols, int UMB_NCLUST){
  // //inicio Parametros:
// int N = 500; //son los 500 puntos sift encontrados. //esta linea es eliminable.
// Mat Puntos(N, 2, CV_32F); 
// randu(Puntos, Scalar(0), Scalar(600)); //La imagen es 600 x 600 píxeles.
// float UMBRAL_MATAR_HUERFANOS = 30;
// float UMB_DIST = 60; //Umbral: Criterio de Paro para el k-means jerarquico.
// int VistaRows  =  80; //Promedio de Altura de las imágenes "Vista"
// int VistaCols  =  90; //Promedio de Anchura de las imágenes "Vista"
// int ImgRows    = 600; // Alto de la imagen.
// int ImgCols    = 600; // Ancho de la imagen.
// int UMB_NCLUST = 20; //Número mínimo de elementos que debe tener el cluster para ser convertido a ventana.
// Fin Parametros.

/// NOTAS: El Main es la funcion, se deben agregar los parametros de forma adecuada. 
/// (los parametros de arriba.

// Mat IMAGEN = Mat::zeros(ImgRows, ImgCols, CV_8UC1);
// imshow("Imagen Negra", IMAGEN);
int N_points = Puntos.rows;
if(N_points <= UMB_NCLUST ){
  Mat dumyWindow(0, 4, CV_32F);
  return dumyWindow;
}
// cout<< "los puntos son: "<<N_points<<endl;
Mat MDists(N_points, N_points, CV_32F); // ¿y si uso CV_64F?
double MD[N_points][N_points];
float ix, iy, jx, jy;
Mat Diff;
for(int i = 0; i < N_points; i++){
  ix = Puntos.at<float>(i,0);
  iy = Puntos.at<float>(i,1);
  for(int j = 0; j < N_points; j++){
    if(i < j){ //Produce una matriz triangular superior.
      jx = Puntos.at<float>(j,0);
      jy = Puntos.at<float>(j,1);
      //MDists.at<float>(i,j) =sqrt( (ix - jx) * (ix - jx) + (iy - jy) * (iy - jy) );
      // MD[i][j] = sqrt( (ix - jx) * (ix - jx) + (iy - jy) * (iy - jy) );
      Diff = Puntos.row(i) - Puntos.row(j);
      //MD[i][j] = sqrt(Diff.dot(Diff));
      MDists.at<float>(i,j) = sqrt(Diff.dot(Diff)); //guarda la distancia en la MAT.
    }// Fin del IF
  } // fin del j-FOR
} // fin del i-FOR

// cout<< "Line 1"<<endl;
//Rellenar la matriz (el triangulo inferior)
for(int i = 0; i < N_points; i++){
  for(int j = 0; j < N_points; j++){
    if(i < j){ //Produce una matriz triangular Inferior
      MDists.at<float>(j,i) = MDists.at<float>(i,j);
    }// Fin del IF
  } // fin del j-FOR
} // fin del i-FOR

//Rellenar la diagonal.
for(int i = 0; i < N_points; i++){
  MDists.at<float>(i,i) = 0; // llena de ceroz la diagonal principal de la matriz. (Evita qe tenga basura).
} //Fin i-FOR

double N_min, N_max;
Point Pmin, Pmax;

// rellenar la diagonal con un numero muy alto.
minMaxLoc(MDists, &N_min, &N_max, &Pmin, &Pmax);
float Infinite = 3 * N_max;
for(int i = 0; i < N_points; i++){
  MDists.at<float>(i,i) = Infinite; // la idea es ponerle +Inf
   // cout << "valor de NMAX: "<< MDists.at<float>(i,i)<<" Y J vale: "<< i<<  endl;
}

//Etapa de Preproceso
// cout<< "Line 2"<<endl;
//Ahora hay que eliminar las filas y Columnas donde el minimo de cada renglón 
Mat Renglon(1, N_points, CV_32F);
int P_candidatos[N_points];
int HowManyPoints = 0;
for(int i = 0; i < N_points; i++){ // Eliminar los renglones de puntos huérfanos. (puntos aislados de los demás puntos SIFT).
  Renglon = MDists.row(i);
  minMaxLoc(Renglon, &N_min, &N_max, &Pmin, &Pmax);
  // cout <<N_min<< ", ";
  if(N_min > UMBRAL_MATAR_HUERFANOS){ //Si el punto vecino más cercano a este punto es más lejano que el umbral...
    P_candidatos[i] = 0; // Eliminar este punto, pues se trata de un punto huérfano. con pocos puntos como vecinos.
  }else{
    P_candidatos[i] = 1; //Es un punto candidato.
    HowManyPoints++;
  }
}
if(HowManyPoints <= UMB_NCLUST ){
  Mat dumyWindow(0, 4, CV_32F);
  return dumyWindow;
}
// Compactar la matriz para que no tenga ya los puntos huérfanos.
// cout << "Publica HowMany Points: "<< HowManyPoints<<endl;
Mat CompactAux(HowManyPoints, N_points, CV_32F);
for(int i = 0, C = 0; i < N_points; i++){
  if(P_candidatos[i]){
    MDists.row(i).copyTo(CompactAux.row(C++)); // Va adquiriendo los renglones correctos.
  }
}
// cout<< "Publica CompactAux: " << CompactAux<< endl;
Mat CompactMatrix(HowManyPoints, HowManyPoints, CV_32F);
Mat CompactPoints(HowManyPoints, 2, CV_32F);
for(int i = 0, C = 0; i < N_points; i++){
  if(P_candidatos[i]){
    CompactAux.col(i).copyTo(CompactMatrix.col(C)); // Va adquiriendo las columnas correctas.
    Puntos.row(i).copyTo(CompactPoints.row(C++));
  }
}

int Etiquetas[HowManyPoints]; //Vector de etiquetas, dice a que cluster pertenecen.
Mat ScoreElements(HowManyPoints, 1, CV_32F, Scalar(1));
for(int i = 0; i < HowManyPoints; i++){
  Etiquetas[i]     = i; //inicialmente hay "HowManyPoints" clústeres.
  //ScoreElements[i] = 1; //inicialmente cada cluster tiene 1 elemento.
}

/* 1. Calcula la distancia minima. en toda la matriz.
 * 2. Crea un vector de Labels del tamaño de la "HowManyPoints" que dirán cual punto es en la matriz.
 * 3. WHILE: Pregunta si esta dist. es menor al umbral. Si sí continua, si no, ahi muere.
 * 3. Crea un nuevo punto que es el centroide de estos dos puntos (coords x & y) (cuidado, quizá estén cruzados)
 * 4. Etiqueta el vector Etiquetas[] a estos dos puntos 
 * 5. Elimina los 2 puntos de la matriz y crea una nueva matriz eliminando las 2 filas y las 2 columnas.
 * 6. Crea un Vector de Labels que dice cual es el nombre original de los elementos de esta nueva matriz. 
 * 7. Saca las distancias de este punto con respecto de los demás.
 * */

minMaxLoc(CompactMatrix, &N_min, &N_max, &Pmin, &Pmax); // 1.
double DistCalculada = N_min;
// int Labels[HowManyPoints]; // 2.
// memcpy((void*) Labels, (void*) Etiquetas, sizeof(int) *  HowManyPoints ) ; //precaución.}

int Px, Py, RealX, RealY;
float NewPoints[HowManyPoints][2];
float NPROW, NPCOL;
int P = 0;
// cout << "Publicamos CompactPoints: " << CompactPoints<< endl;
//cout<< "Line 3"<<endl;
while((DistCalculada < UMB_DIST) && (CompactPoints.rows > 2)){ // 3
  Px = Pmin.x;
  Py = Pmin.y;
//   RealX = Labels[Px];
//   RealY = Labels[Py];
//   if(Etiquetas[RealX] < Etiquetas[RealY]){ // 4
//     Etiquetas[RealY] = Etiquetas[RealX]; //pone la etiqueta más baja.
//   }else{
//     Etiquetas[RealX] = Etiquetas[RealY]; //pone la etiqueta más baja.
//   }
//   cout<< "While 1"<<endl;
  NPROW = (CompactPoints.at<float>(Px, 0) + CompactPoints.at<float>(Py, 0) ) / 2.0; //fusionando coordenadas.
  NPCOL = (CompactPoints.at<float>(Px, 1) + CompactPoints.at<float>(Py, 1) ) / 2.0; //Fusionando coordenadas.
  //cout<< "Nuevo Punto es: " << NPROW << " y " << NPCOL << endl;
  // cout<< "Coordenadas son: " << Px << " y " << Py << endl;
//   cout<< "While 2"<<endl;
  CompactMatrix = QuitarPuntos(CompactMatrix, Px, Py, CompactPoints); // 5 en realidad, esta es una matriz de Clusteres.
//   cout<< "While 3"<<endl;
  ScoreElements = RecaulcularScore(ScoreElements, Px, Py); //quita los 2 elementos, y sus valores los suma al final. (para saber cuantos elementos tiene cada cluster.
//   cout<< "While 4"<<endl;
  CompactMatrix = RecalcularDistancia_NewPoint(CompactMatrix, NPROW, NPCOL, CompactPoints, Infinite); // 7
//   cout<< "While 5"<<endl;
  
  minMaxLoc(CompactMatrix, &DistCalculada, &N_max, &Pmin, &Pmax); // Repetir....
  // Se sigue el ciclo hasta salirse.
  // cout<< "la distancia minima ahora es: " << DistCalculada<< endl;
}
// 1. Ahora preguntar cuantos clusteres superan el umbral.
// cout<< "Publicamos Score: " << ScoreElements<< endl;
// cout<< "Line 4"<<endl;
int N_Clusteres = CompactMatrix.rows;
//cout << "Salieron "<< N_Clusteres << " Clusteres" << endl;

int rowini, rowend, colini, colend;
double t;
float VistaR = (float) VistaRows / 2.0;
float VistaC = (float) VistaCols / 2.0;
Mat Ventanas(N_Clusteres, 4, CV_32F);
int Puntero = 0 ; //Significa Vacio.

for(int i = 0; i < N_Clusteres; i++){
  if(ScoreElements.at<float>(i) > UMB_NCLUST){
    //crear las coordenadas de la ventana.
    // ¿Preguntar si la std de estos puntos es congruente con las dimensiones del objeto a detectar?
    //por ahora no.
    rowini = (t = CompactPoints.at<float>(i,0) - VistaR) < 0 ? 0: (int)t;
    rowend = (t = CompactPoints.at<float>(i,0) + VistaR) > ImgRows ? ImgRows: (int)t;
    colini = (t = CompactPoints.at<float>(i,1) - VistaC) < 0 ? 0: (int)t;
    colend = (t = CompactPoints.at<float>(i,1) + VistaC) > ImgCols ? ImgCols: (int)t;
    AddWindow(Ventanas, rowini, rowend, colini, colend, ++Puntero);
    //cout << CompactPoints.at<float>(i,0) << " " << CompactPoints.at<float>(i,1) << endl;
  }else{
    //No crear ventana.
  }
}
// Mat Ventanas(N_Clusteres, 4, CV_32F);
Ventanas = CompactarVentanas(Ventanas, Puntero); //las pone como debe de ser.
//cout << "Salieron "<< Puntero << " Ventanas"<<endl;
//cout << "las coordenadas de las ventanas son: "<<endl<< Ventanas<< endl;
// el resultado está en etiquetas.

// return Ventanas; // DESCOMENTAR ESTO, lo que devuelve es Ventanas, una MAT de N x 4 que contiene las coordenadas de las ventanas
// ORDEN: ROWINI, ROWFINAL, COLUMNAINI, COLUMNAFINAL
return Ventanas;
}

Mat RECOGNITION:: QuitarPuntos(Mat& CompactMatrix, int Pcol, int Prow, Mat &CompactPoints){
  // 5 en realidad, esta es una matriz de Clusteres.
  int N = CompactPoints.rows;
  // int N2 = CompactMatrix.cols;
  // cout << "deben ser iguales: " << N << " y " << N2<<endl;
  Mat NewCompact(N - 2, 2, CV_32F);
  Mat NewMatrix(N - 2, N -2, CV_32F);
  Mat T1(N - 2, N, CV_32F);
   // cout << "deben ser coincidir : " << CompactMatrix.cols << " y " << CompactMatrix.rows << " y " << CompactPoints.rows << endl;
  for(int i = 0, C = 0; i < N; i++){
    if((i != Pcol) && (i != Prow)){ // Si No es un punto a eliminar, (RECUERDE LEYES DE DE MORGAN
      // NewCompact.row(C)  = CompactPoints.row(i); //compacta los centroides de los clusteres.
      CompactPoints.row(i).copyTo(NewCompact.row(C)); 
      // NewMatrix.row(C++) = CompactMatrix.row(i); //compacta renglones de matriz de distancias.
      CompactMatrix.row(i).copyTo(T1.row(C++));
      // cout << "jala: "<< i<< "esto vale C: " << C<<endl;
    }
  }
  // cout << "JAJAJAJAJAJAJAJA deben ser enteros : " << Pcol << " y " << Prow <<endl;
  for(int i = 0, C = 0; i < N; i++){
    if((i != Pcol) && (i != Prow)){ // Compacta las columnas de la Matriz de distancias.
      //NewMatrix.col(C++) = CompactMatrix.col(i);
      T1.col(i).copyTo(NewMatrix.col(C++));
    }
  }
   // cout << "hola hola"<<endl;
   CompactPoints = NewCompact; //sustituye los puntos.
   // cout << "hola2222 hola"<<endl;
  return NewMatrix; //sustituye la matriz de distnacias.
}

Mat RECOGNITION:: RecaulcularScore(Mat& ScoreElements, int Pcol, int Prow){
  int N = ScoreElements.rows;
  Mat NewMatScore(N - 1, 1, CV_32F);
  
  for(int i = 0, C = 0; i < N; i++){
    if((i != Pcol) && (i != Prow)){ // Compacta el vector de Scores.
      NewMatScore.at<float>(C++) = ScoreElements.at<float>(i);
    }
  }
  //cout<< "Sumandos son: " << ScoreElements.at<float>(Pcol) <<" y " <<ScoreElements.at<float>(Prow)<< endl;
  NewMatScore.at<float>(N - 2) = ScoreElements.at<float>(Pcol) + ScoreElements.at<float>(Prow);
  //quita los 2 elementos, y sus valores los suma al final. (para saber cuantos elementos tiene cada cluster.
  return NewMatScore;
  //cout <<"Vector de Scores en cada iteracion: "<< NewMatScore<< endl;
  
}

Mat RECOGNITION:: RecalcularDistancia_NewPoint(Mat& CompactMatrix, float NewProw, float NewPcol, 
					       Mat& CompactPoints, float& Infinite){
  // 1. Voy a agregar los nuevos puntos a CompactPoints
  // 2. Voy a añadir una fila y una columna a CompactMatrix y la voy a devolver en la funcion.
  int r = CompactPoints.rows;
  // Mat DistANewPoint(1, r, CV_32F);
  
//    cout<< " POINTS ANTES: " << r << endl;
  Mat NewC; // (r + 1, 2,CV_32F);
  Mat T1(1, 2, CV_32F);
  vconcat(CompactPoints ,T1, NewC);
  // NewC = CompactPoints.reshape(0, r);
  r = NewC.rows;
  
  
  NewC.at<float>(r - 1, 0) = NewProw;
  NewC.at<float>(r - 1, 1) = NewPcol;
  Mat Diff;
  float Dist;
   // cout<< " POINTS DESPUES (DEBE SUBIR UNO): " << r << endl;
  CompactPoints = NewC;
  Mat LastRenglon(1, r - 1, CV_32F);
  for(int i = 0; i < (r - 1); i++){
    Diff = NewC.row(i) - NewC.row(r - 1);
    // cout<< "Pub DIFF: "<< Diff<< endl;
    Dist = sqrt( Diff.dot(Diff));
    LastRenglon.at<float>(0, i) = Dist;
  }
  Mat NewMatrix, Temp1;
  
  
  vconcat(CompactMatrix, LastRenglon, Temp1);
//    cout << "AHI VA"<<endl;
  Mat LastColumn = LastRenglon.t();
  Mat T2(1,1,CV_32F);
//    cout <<"DEBE ChANGE: " <<LastColumn.rows<<endl;
  vconcat(LastColumn, T2, LastColumn);
//    cout <<"DEBE ChANGE PLUS: " <<LastColumn.rows<<endl;
  LastColumn.at<float>(r - 1 , 0) = Infinite;
  hconcat(Temp1, LastColumn, NewMatrix);
//    cout << "QUE SHOW"<< LastColumn << endl;;
  
  return NewMatrix;
}

void RECOGNITION:: AddWindow(Mat& Ventanas, int& rowini, int& rowend, int& colini, int& colend, int Puntero){
  Ventanas.at<float>(Puntero - 1, 0) = (float) rowini;
  Ventanas.at<float>(Puntero - 1, 1) = (float) rowend;
  Ventanas.at<float>(Puntero - 1, 2) = (float) colini;
  Ventanas.at<float>(Puntero - 1, 3) = (float) colend;
}

Mat RECOGNITION:: CompactarVentanas(Mat CompactV, int p){
  // Parece una funcion lenta...
  Mat NewWindow(p, 4, CV_32F);
  for(int i = 0; i < p; i++){
    //NewWindow.row(i) = CompactV.row(i);
    CompactV.row(i).copyTo(NewWindow.row(i));
  }
  return NewWindow;
}


OBJECT::OBJECT(){
}

void OBJECT::getDescriptorsViews(int objs, int nclassifiers){
  VistaRows = 0 ; // prom de rows de imgs de entrenamiento
  VistaCols = 0; // prom de cols de imgs de entrenamiento
  std::vector<KeyPoint> keypoints;
  Mat descriptor;
  
  cv::SiftFeatureDetector detector(0.04, 10.0);
  cv::SiftDescriptorExtractor extractor (3.0); 
  
    for(int  j = 0; j < nclassifiers; j++){
      std::stringstream file;
      file << "./concept"<< objs <<"/classifier" << j + 1 << "/classifier" << j + 1 <<  ".png";
      cout << file.str() << endl;
      Mat view  = cv::imread( file.str(), CV_LOAD_IMAGE_GRAYSCALE );
      VistaRows += view.rows;
      VistaCols += view.cols;
      //imshow("view", view);
      //cvWaitKey(0);
      detector.detect(view, keypoints);
      extractor.compute( view, keypoints, descriptor); 
      descriptorsViews.push_back(descriptor);
    }
  
  VistaRows /= nclassifiers;
  VistaCols /= nclassifiers ;
}

void OBJECT:: loadClassifiers(string name, int number, int &nclassifiers){
  //getParams(number);
  CLASSIFIERS classifier1[nclassifiers];
  for(int i = 0; i < nclassifiers; i++){
    classifier1[i].loadData(number, i + 1);
    listClassifier.push_back(classifier1[i]);
  }
}


vector<double> OBJECT::sendFeatures( Mat &featuresL, int &nclassifiers){
  //vector<double> PG;
  vector<double> PL;
  for(int i = 0 ; i < nclassifiers; i++){
    //listClassifier[i].getProbability(featuresG,featuresL, PG, PL);
    listClassifier[i].getProbability(featuresL, PL);
  }
   vector<double>  objectRec = fusionProbabilities(PL);
   return objectRec;
}

void OBJECT:: getParams( int n){
  Mat m;
  std::stringstream path;
  path << "./concept" << n << "/params.yml";
  FileStorage fs(path.str(), FileStorage::READ);
  fs["params"] >> m;
  L = m.at<float>(0,0); //lambda
  F = m.at<float>(0,1); //votado
  E = m.at<float>(0,2); //etha
  //cout << L << " " << F << " " << E << endl;
  fs.release();
}

vector<double> OBJECT:: fusionProbabilities(vector<double> &PL){
  vector<double> objectRec;
  int classifiers = PL.size();
  double storeProb = 0;
  vector<double> fusionP(classifiers, 0.0);
  for(int i = 0; i < classifiers; i++ ){
     //cout<< "fusion " << ( L * PG[i] ) + ( (1.0 - L) * PL[i] ) << endl ;
     //cout<< "fusion " <<  PL[i]  << endl ;
    //if (( L * PG[i] ) + ( (1-L) * PL[i] ) >= (E - 0)){
    if (PL[i]  >= (0.65 - 0)){ //E
      //storeProb += ( L * PG[i] ) + ( (1-L) * PL[i]);
      storeProb +=  PL[i];
      fusionP[i] = 1;
    //cout<< "fusion mayor" << ( L * PG[i] ) + ( (1.0 - L) * PL[i] ) << endl ;
    }
    else
      fusionP[i] = 0;
  }
 
  double rec  = sum(fusionP)[0];
  double prob= storeProb / rec;
  if ( rec >= 1) { //F
    objectRec.push_back(1);
    objectRec.push_back(prob);
  }
  else{
    objectRec.push_back(0);
    objectRec.push_back(0);
  }
  //cout << objectRec[1] << endl;
  //cout << objectRec[0] << endl;
  return objectRec;
}

// void OBJECT::sendRectangle(int existObject, int pt1x, int pt1y, int windowX, int windowY){
// 		      
//   int endX = pt1x + windowX;
//   int endY = pt1y + windowY;
//   ////cout <<"windowX " << windowX << " windowY "  << windowY << endl;
//   //cout << "pt1x "  << pt1x << " pt1y "  << pt1y << " endX " << endX << " endY " << endY << endl;
//   if (existObject == 1)  {
//     //cout << "pt1x "  << pt1x << " pt1y "  << pt1y << " endX " << endX << " endY " << endY << endl;
//     for(int i = pt1x; i < endX; i++){
//       for(int j = pt1y; j < endY; j++){
// 	matrizFrec.at<float>(j,i) = matrizFrec.at<float>(j,i) + 1;
//       }
//     }
//   }
// }


void OBJECT::drawRectangle(Mat &image, int pt1x, int pt1y, int pt2x, int pt2y, Mat &coordinate, int concepts, string name){
    IplImage *src=NULL;
    IplImage copy = image;
    src = &copy;
    
    CvPoint pt1, pt2;
    pt1.x = pt1x; //coordenada x
    pt1.y = pt1y; //coordenada y
    pt2.x = pt2x;
    pt2.y = pt2y;
    
    //calcular centroreco
    int centerX = (int)((pt1x + pt2x) / 2.0);
    int centerY = (int)((pt1y + pt2y) / 2.0);
    
    coordinate.at<float>(concepts,0) = pt1x;
    coordinate.at<float>(concepts,1) = pt1y;
    
    coordinate.at<float>(concepts,2) = pt2x;
    coordinate.at<float>(concepts,3) = pt2y;
    
    CvFont font; 
    const char *obj;
    obj = name.c_str();
    cvInitFont( &font, CV_FONT_VECTOR0, 0.8, 0.8, 0, 2.0, CV_AA); //Inicializamos el código fuente 
    cvPutText(src, obj, pt2, &font,CV_RGB(230, 95, 0) ); 
    cvRectangle( src, pt1 , pt2 , CV_RGB(0,0,255), 3, 8, 0 ); //Dibujamos el rectangulo  
    cvWaitKey(100);
    Mat w = src;
    std::stringstream path;
    path << "../data/" << name << ".png";
    imwrite(path.str(), w);
    cout << "CONCEPTO " << name << " encontrado .:. GUARDARNDO IMAGEN .:. " << endl;
    //cvShowImage("Ejemplo", src);
    //cvWaitKey(0);
}

void OBJECT::drawRectangle(Mat &image, int pt1x, int pt1y, int pt2x, int pt2y, Mat &coordinate, int concepts, string name, Mat &h){
    int  OutPutRow, OutPutCol;  
    IplImage *src=NULL;
    IplImage copy = image;
    src = &copy;
    
    CvPoint pt1, pt2;
    pt1.x = pt1x; //coordenada x
    pt1.y = pt1y; //coordenada y
    pt2.x = pt2x;
    pt2.y = pt2y;
    
    //calcular centro
    int centerX = (int)((pt1x + pt2x) / 2.0);
    int centerY = (int)((pt1y + pt2y) / 2.0);
    TransformarPunto(centerY, centerX, OutPutRow, OutPutCol, h);
    coordinate.at<float>(concepts,0) = (float) OutPutCol;
    coordinate.at<float>(concepts,1) = (float) OutPutRow;
    
    coordinate.at<float>(concepts,2) = (float) pt1x;
    coordinate.at<float>(concepts,3) = (float) pt1y;

    coordinate.at<float>(concepts,4) = (float) pt2x;
    coordinate.at<float>(concepts,5) = (float) pt2y;
    
}

// void OBJECT::drawRectangle(Mat &image, Mat &coordinate, int concepts){
// //void OBJECT::drawRectangle(Mat &image){
// 
//   ofstream myFile;
//   myFile.open("matrizFrec.csv");
//   myFile << format(matrizFrec,"csv") << endl << endl;
//   
//   double max= *std::max_element(matrizFrec.begin<float>(), matrizFrec.end<float>());
//   cout << "MAX " << max << endl;
//   if (max >= 4){
//     Mat result, locations;
//     inRange(matrizFrec, max - 3, max, result);
//       
//     //cout << image.rows << image.cols << endl;
//     //cout << matrizFrec.rows << matrizFrec.cols << endl;
//     //cout << result.rows << result.cols << endl;
//     
//     vector<Mat> planes; 		/// Separate the image in 3 places ( B, G and R )
//     split( image, planes );
//     Mat plane1 = planes[1] + result;
//     
//     cv::findNonZero(result, locations);
//     int ptFinal = locations.rows;
//   //   cout << "pt1 " << locations.at<int>(0,0) << " " << locations.at<int>(0,1) << endl;
//   //   cout << "pt2 " << locations.at<int>(ptFinal -1, 0) << " " << locations.at<int>(ptFinal -1,1) << endl;
// 
//     IplImage *src=NULL;
//     IplImage copy = image;
//     src = &copy;
//     
//     CvPoint pt1, pt2;
//     pt1.x = locations.at<int>(0,0); //coordenada x
//     pt1.y = locations.at<int>(0,1); //coordenada y
//     pt2.x = locations.at<int>(ptFinal -1, 0);
//     pt2.y = locations.at<int>(ptFinal -1, 1);
//     coordinate.at<int>(concepts,0) = pt1.x;
//     coordinate.at<int>(concepts,1) = pt1.y;
//     coordinate.at<int>(concepts,2) = pt2.x;
//     coordinate.at<int>(concepts,3) = pt2.y;
//     
//     cvRectangle( src, pt1 , pt2 , CV_RGB(0,0,255), 3, 8, 0 ); //Dibujamos el rectangulo
//   //   
//   //   //imshow("plane", plane1);
//     cvShowImage("Ejemplo", src);
//     cvWaitKey(0); //se queda esperando a que se presione una tecla
//   //   cvReleaseImage( &src); //se libera el recurso de memoria donde está la imagen
//   //   cvDestroyWindow("Ejemplo" ); //destruimos la ventana
//   }
//   
// }

CLASSIFIERS::CLASSIFIERS(){
}

void CLASSIFIERS:: loadData(int n, int i){
  //caragar yml
  //getModelsNB( n, i);
  getDescriptors( n, i);
}

void CLASSIFIERS:: getModelsNB(int n, int i){
  
    std::stringstream path;
    path << "./concept" << n << "/classifier" << i << "/ModeloBayes.yml"; 
    //cout << path.str() << endl;
    FileStorage fs(path.str(), FileStorage::READ);
    if (!fs.isOpened()) cout << "Concept not found" << endl;
    fs["TPC_HistP"] >> TPC_HistP;
    fs["TPC_HistN"] >> TPC_HistN;
    fs["RangoHP"]   >> RangoHP;
    fs["RangoHN"]   >> RangoHN;
    fs.release();
}

void CLASSIFIERS:: getDescriptors(int n, int i){
  
    Mat m;
    std::stringstream path;
    path << "./concept" << n << "/classifier" << i <<  "/descriptor.yml";
    //cout << path.str() << endl;
    FileStorage fs(path.str(), FileStorage::READ);
    if (!fs.isOpened()) cout << "Concept not found" << endl;
    fs["metrica"] >> m;
    fs["descriptor"] >> descriptors;
    metric = m.at<float>(0,0);
    fs.release();
} 

void CLASSIFIERS::getProbability( Mat &featuresL, vector<double> &PL){
//     double Smp[212];
//     for(int j = 0; j < 212; j++)
//       Smp[j] = featuresG[j];
//     //Ahora convertimos a muestra en evidencia.
//       // son CUATRO estados de manera constante.
//     int EvP[212];
//     int EvN[212];
//     float P1, P2, P3;
//     float N1, N2, N3;
//     
//     for(int j = 0; j < 212; j++)
//     {
//       P1 = RangoHP.at<float>(0,j);
//       P2 = RangoHP.at<float>(1,j);
//       P3 = RangoHP.at<float>(2,j);
//       N1 = RangoHN.at<float>(0,j);
//       N2 = RangoHN.at<float>(1,j);
//       N3 = RangoHN.at<float>(2,j);
//       if (Smp[j] < P1)
// 	EvP[j] = 0;
// 	else if ((Smp[j] >= P1) && (Smp[j] < P2))
// 	  EvP[j] = 1;
// 	else if ((Smp[j] >= P2) && (Smp[j] < P3))
// 	  EvP[j] = 2;
// 	  else if (Smp[j] >= P3)
// 	    EvP[j] = 3;
// 	  
// 	  
//     if (Smp[j] < N1)
// 	EvN[j] = 0;
// 	else if ((Smp[j] >= N1) && (Smp[j] < N2))
// 	  EvN[j] = 1;
// 	else if ((Smp[j] >= N2) && (Smp[j] < N3))
// 	  EvN[j] = 2;
// 	  else if (Smp[j] >= N3)
// 	    EvN[j] = 3;
//       }
//       
//     // Ejecutar Naive Bayes. Preguntar Probabilidad.
//     //for(int j=0; j < 212; j++)
//       // cout<< "Evidencia P es: "<< EvP[j]<<endl;
//     double LY = 1;
//     double LN = 1;
//     double temp1;
//     for(int j = 0; j < 212; j++)
//     {
//       LY = LY * TPC_HistP.at<float>(EvP[j], j);
//       LN = LN * TPC_HistN.at<float>(EvN[j], j);
//       //cout << EvP[j] << " " << j << endl;
//       // Normalizar para evitar destrucción de la probabilidad.
//       temp1 = LY + LN;
//       // cout<< "TPCP es : "<< LY <<endl;
//       LY = LY / temp1;
//       LN = LN / temp1;
//     }
//     
//     //cout<<"Probab. de SI objeto: "<< LY<<endl;
//     probG = LY;
//     PG.push_back(probG); 
    
    //insertar la funcion vl_ubcmatch
     
    vector<int> matches = vl_ubcmatch(featuresL,descriptors);
    
    vector<int> result;
    double probL;
    //cout << "featuresL " << featuresL.rows << " " << featuresL.cols << endl;
    //cout << "descriptors " << descriptors.rows << " " << descriptors.cols << endl;
      
    sort(matches.begin(),matches.end());
    //cout << "matches "<< matches.size()  << endl;

    
//     for(int j = 0; j < matches.size(); j++){  // encontrar la mejor manera de hacer esto
//       result.push_back(matches[j]);
//     }
   
    vector<int>::iterator pos;
    pos = find_if (matches.begin(), matches.end(),  // range
		    bind2nd(greater<int>(), metric));  // criterion
    double limit = distance(matches.begin(),pos);
    //cout << "limit " << limit <<  " metric " << metric << endl;
    vector<double> t;
    //result.resize(limit);
    //divide(result,sum(result)[0], t );
    //cout << "result "<< result.size() << " prom " << sum(result)[0]/ limit << endl;
    if (limit != 0)
      probL = 1 - (1 /(limit + 1) );
    else
      probL = 0.0001;
      
    PL.push_back(probL);
    
    int sum = 0;
    for( int i = 0; i < matches.size(); i++ ){ 
	  sum = matches[i] + sum;
	//  cout<< "distancia "<< i << " "<< dist<< endl;
    //if( dist < min_dist ) min_dist = dist;
    //if( dist > max_dist ) max_dist = dist;
	}
    //cout<< "distancia  "<<  sum << "/"<< matches.size() <<" = " <<  sum / matches.size() << endl;
    //cout << "probability- Local " << probL << endl;   
    
}

vector<int> CLASSIFIERS::vl_ubcmatch(Mat &featuresL, Mat &descriptors){

  double UMBRAL = 1.3; //Este es el umbral, puede ser considerado como parámetro.
  
  int N_feats = featuresL.rows; 
  int N_descripts = descriptors.rows; 
  Mat Values(N_feats, N_descripts, CV_32F);
  Mat Diff;
  
  vector<int> matching;
  double N_min, N_max, VV;
  Point Pmin, Pmax;
  
  for (int r = 0; r < N_feats; r++)
    for (int c = 0; c < N_descripts; c++){
      Diff = featuresL.row(r) - descriptors.row(c);
      VV = Diff.dot(Diff);
      Values.at<float>(r,c) = sqrt(VV); //Guarda TOdas las distancias en una Matriz.
    }
    float temp;
    for (int r = 0; r < N_feats; r++){ //Evalua tipo UbcMatch cada renglón.
      minMaxLoc(Values.row(r), &N_min, &N_max, &Pmin, &Pmax); //obtiene los minimos de distancias
      Values.at<float>(r, Pmin.x) = N_max + 1 ;
      temp = N_min;
      minMaxLoc(Values.row(r), &N_min, &N_max, &Pmin, &Pmax); //obtiene los minimos de nuevo
       // cout<< "publico el menor: " << temp << " y el que le sigue: "<< N_min<< endl;
      if((temp * UMBRAL) < N_min)
      {
	matching.push_back((int)temp);
	//cout << "temp " << temp << endl;
      }
    }
    
    return matching;
}
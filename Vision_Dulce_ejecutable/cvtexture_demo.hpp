#ifndef __CVTEXTURE_DEMO__
#define __CVTEXTURE_DEMO__

#include <opencv/cv.h>

using namespace cv;

class GLCM {
  
public:
  GLCM();
  //~LOCAL();
  void getGLCM(Mat &src, vector<double> &globalFratures );
};

#endif
#include <opencv/cv.h>

extern "C" {
  #include "vl/generic.h"
}

using namespace cv;

int main(){
  
  Mat image = imread("k3.png", CV_LOAD_IMAGE_UNCHANGED); //imagen de prueba
  
  int width = image.cols;
  int height = image.rows;
  int noctaves = 8;
  int nlevels = 3;
  int o_min = 0; // Set the index of the first octave of the DoG scale space.
  
  VlSiftFilt* vl_sift_new( width, height, noctaves, nlevels, o_min);
  
  return 0:
}
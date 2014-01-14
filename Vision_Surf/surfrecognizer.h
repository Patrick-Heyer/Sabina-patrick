/*
    Class to abstract the implementation of SURF and K-D Trees for object Recognition.
    Based on the example of opencv for object recongnition by Liu Liu.
    Copyright (C) 2012  J. Irving Vasquez ivasquez@ccc.inaoep.mx

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef SURFRECOGNIZER_H
#define SURFRECOGNIZER_H

#include <string>
#include <iostream>

#include <cv.h> 
#include <highgui.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

   static CvScalar colors[] = 
    {
        {{0,0,255}},
        {{0,128,255}},
        {{0,255,255}},
        {{0,255,0}},
        {{255,128,0}},
        {{255,255,0}},
        {{255,0,0}},
        {{255,0,255}},
        {{255,255,255}}
    };

    
/**
 * Class to abstract the implementation of SURF and K-D Trees for object Recognition.
 * Based on the example of opencv for object recongnition.
 */
    
class SurfRecognizer
{

public:
  
  SurfRecognizer();
  
  ~SurfRecognizer();

  void setObjectFileName(string filename);
  
  bool loadObjectFile();
  
  bool recongnizeObjectInImage(IplImage *imageWithObject);
  
  void setDisplaying(bool value);
  
  void setRecognitionTreshold(int value);
  
  void setObjectName(string name);
  
  string getObjectFileName();
  
  virtual void displayObject();

private:
  string objectName;
  
  int recTreshold;
  
  bool objectFounded;
  
  bool windowOpened;
  
  IplImage *object;
  
  CvMemStorage* storage;
  
  CvSeq *objectKeypoints, *objectDescriptors;
  
  CvSURFParams params;
  
  CvPoint src_corners[4];
  CvPoint dst_corners[4];
  
  string objectFileName;
  bool displaying;
  
  double compareSURFDescriptors( const float* d1, const float* d2, double best, int length);
  
  int naiveNearestNeighbor( const float* vec, int laplacian,
                      const CvSeq* model_keypoints,
                      const CvSeq* model_descriptors );
  
  void findPairs( const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
           const CvSeq* imageKeypoints, const CvSeq* imageDescriptors, vector<int>& ptpairs );
  
  void flannFindPairs( const CvSeq*, const CvSeq* objectDescriptors,
           const CvSeq*, const CvSeq* imageDescriptors, vector<int>& ptpairs );
  
  int locatePlanarObject( const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
                    const CvSeq* imageKeypoints, const CvSeq* imageDescriptors,
                    const CvPoint src_corners[4], CvPoint dst_corners[4] );
};

#endif // SURFRECOGNIZER_H

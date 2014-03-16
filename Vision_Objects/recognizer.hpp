/*
    Class to abstract the implementation of SURF and K-D Trees for object Recognition.
    Based on the example of opencv for object recongnition by Liu Liu.
    Copyright (C) 2014  J. Irving Vasquez ivasquez@ccc.inaoep.mx and
    Elias Ruiz elias_ruiz@inaoep.mx

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


#ifndef ORBRECOGNIZER4_HPP
#define ORBRECOGNIZER4_HPP
#define USE_FLANN 1

#include <string>
#include <iostream>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/core/core.hpp>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "HC.hpp"
using namespace cv;
using namespace std;

static CvScalar colors[] =
{
    {{0,0,255}},   //0 - RED
    {{0,128,255}}, //1 - ORANGE
    {{0,255,255}}, //2 - YELLOW
    {{0,255,0}},   //3 - GREEN
    {{255,128,0}}, //4 - DARK CYAN
    {{255,255,0}}, //5 - CYAN
    {{255,0,0}},   //6 - BLUE
    {{255,0,255}}, //7 - MAGENTA
    {{255,255,255}} //8 - WhITE
};


/**
 * Class to abstract the implementation of ORB and K-D Trees for object Recognition.
 * Based on the example of opencv for object recongnition.
 */

class OrbRecognizer
{

public:
    OrbRecognizer();
    ~OrbRecognizer();

    void setObjectFileName(string filename);
    void setThreshold(float value);
    bool loadObjectFile();
    bool recongnizeObjectInImage(IplImage *imageWithObject, int &X, int &Y);
    bool evalWindow(Mat Ventanita);
    void setDisplaying(bool value);
    void setRecognitionTreshold(int value);
    void setMachine(int value);
    void setDistance(int value);
    void setNumMaxTrainingSet(int value);
    void setObjectName(string name);
    void TrainingSet(string dir);
    void ExecuteTraining();
    string getObjectFileName();

private:
    ORB*  OrbMachine;
    ORB*  OrbMachine2;
    SIFT* SiftMachine;
    SURF* SurfMachine;
    string objectName;
    string objectFileName;
    int recTreshold;
    int TypeMachine;
    int TypeDistance;
    bool objectFounded;
    bool objectFoundedV;
    bool windowOpened;
    bool displaying;
    IplImage* object;
    IplImage* objectColor;
    float nndrRatio;
    vector<KeyPoint> objectKeypoints;
    vector<KeyPoint>** trainKeypoints;
    Mat* objectDescriptors;
    Mat** trainDescriptors;
    int NumTrainSet;
    int NumMaxTrainingSet;

    double compareORBDescriptors( const float* d1, const float* d2, double best, int length);
    bool ExecuteTesting(vector<KeyPoint> recorteKeypoints, Mat recorteDescriptors);

    bool flannFindPairs( vector<KeyPoint> objectKeypoints, Mat objectDescriptors,
                         vector<KeyPoint> imageKeypoints,  Mat imageDescriptors,
                         vector<Point2f>& mpts_1,  vector<Point2f>& mpts_2, // Used for homography
                         vector<int>& indexes_1, vector<int>& indexes_2,
                         Mat& outlier_mask, vector<Point2f>& scene_corners, vector<DMatch>& matches);
    bool FindPairs(vector<KeyPoint> objectKeypoints, Mat objectDescriptors,
                   vector<KeyPoint> imageKeypoints,  Mat imageDescriptors,
                   vector<Point2f>& mpts_1, vector<Point2f>& mpts_2,
                   vector<int>& indexes_1, vector<int>& indexes_2);
};

#endif // ORBRECOGNIZER4_H

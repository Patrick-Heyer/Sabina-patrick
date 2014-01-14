#ifndef MATCHINGSURF_H
#define MATCHINGSURF_H
#include <vector>

#include <opencv2/features2d/features2d.hpp>
#include <opencv/cvaux.h>
#include <opencv/cxcore.h>
#include <opencv/cxmisc.h>
#include <opencv/cv.h>
#include "imagesimilarity.h"
//#include <QDebug>

#define NO_NEIGHBOR -1

using namespace std;

class MatchingSURF
{
public:
    MatchingSURF();
    double compareSURFDescriptors(const float *image1Descriptor, const float *image2Descriptor,int descriptorsCount);


    vector<vector<CvPoint2D32f> >findMatchingKeyPoints(CvSeq* image1KeyPoints, CvSeq* image1Descriptors, CvSeq* image2KeyPoints, CvSeq* image2Descriptors,vector<int>& indices1,vector<int>& indices2 );
    vector<vector<CvPoint2D32f> >findMatchingKeyPoints_1(CvSeq* image1KeyPoints, CvSeq* image1Descriptors, CvSeq* image2KeyPoints, CvSeq* image2Descriptors,vector<int>& indicesKeyPoints2,vector<int>& indices1,vector<int>& indices2 );

    vector<vector<CvPoint2D32f> >findMatchingKeyPoints2(CvSeq* image1KeyPoints, CvSeq* image1Descriptors, CvSeq* image2KeyPoints, CvSeq* image2Descriptors, vector<int>& indices1,vector<int>& indices2 );

    vector<vector<CvPoint2D32f> >findMatchingKeyPoints2_1(CvSeq* image1KeyPoints, CvSeq* image1Descriptors, CvSeq* image2KeyPoints, CvSeq* image2Descriptors,vector<int>& indicesKeyPoints2, vector<int>& indices1,vector<int>& indices2 );


    double findHomography(CvSeq* image1KeyPoints, CvSeq* image1Descriptors, CvSeq* image2KeyPoints, CvSeq* image2Descriptors);

    double findHomography(CvSeq* image1KeyPoints, CvSeq* image1Descriptors, CvSeq* image2KeyPoints, CvSeq* image2Descriptors, vector<int>& indicesKeyPoints2);

    void pointsInRect( CvSeq *scenetKeyPointComplete,
                       CvSeq *sceneDescriptorsComplete,
                       vector<int>& indices, CvRect rect);

private:
    double compareSURFDescriptors(
        const float* image1Descriptor,
        const float* image2Descriptor,
        int descriptorsCount,
        float lastMinSquaredDistance);

    int findNaiveNearestNeighbor(const float* image1Descriptor, const CvSURFPoint* image1KeyPoint, CvSeq* image2Descriptors, CvSeq* image2KeyPoints);

    int findNaiveNearestNeighbor_1(const float* image1Descriptor, const CvSURFPoint* image1KeyPoint, CvSeq* image2Descriptors, CvSeq* image2KeyPoints, vector<int> indices2);



};

#endif // MATCHINGSURF_H

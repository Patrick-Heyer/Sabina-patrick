#ifndef FINDOBJECT_H
#define FINDOBJECT_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "histograma.h"
#include "imagesimilarity.h"
#include "imagelocation.h"
#include "comparepoint.h"
#include "toolsimages.h"
#include <queue>

 class FindObject
{
public:
     //images should be in HSV

   //Find by Histogramsxy

    bool FindByHistograms(IplImage* scene,   cv::Mat*& histo_template, IplImage* msk_temp, ImageLocation**& pointOut,int& numOut,int samples_width, int samples_Height, int numScalas, int numHistox, int numHistoy, int nh, int ns, int nv,bool spaceColorC1c2c3);
    bool FindBySpacial(IplImage* scene,   IplImage* templa, IplImage* msk_temp, ImageLocation**& pointOut,int numOut,int samples_width, int samples_Height, int numScalas,bool spaceColorC1c2c3, int medidaSimilitud, int numCanalColor,int numCuantificacion);

   bool FindByHistogramaXY_define(IplImage* scene, cv::Mat*& histo_template,IplImage* msk_template,ImageLocation**& pointsIn,int numIn,ImageLocation**& pointsOut,int numout, int numHistox, int numHistoy, int nh, int ns, int nv, bool spaceColorC1c2c3);
    bool FindBySpacial_define(IplImage* scene, IplImage* templa,IplImage* msk_template,ImageLocation**& pointsIn,int numIn,ImageLocation**& pointsOut,int numout,bool spaceColorC1c2c3, int medidaSimilitud, int numCanalColor,int numCuantificacion);
    bool FindBySURF_define(IplImage* scene,  CvSeq* objectKeyPoint, CvSeq* objectDescriptors,ImageLocation**& pointsIn,int numIn,ImageLocation*& pointOut);


    bool FindBySURF_define(CvSeq* sceneKeyPoint, CvSeq* sceneDescriptors,  CvSeq* objectKeyPoint, CvSeq* objectDescriptors,ImageLocation**& pointsIn,int numIn,ImageLocation*& pointOut);

    //auxiliar methods
   void drawRect(IplImage* img, CvPoint pini, CvPoint pfin, CvScalar color);
   IplImage* subImage(IplImage* src,CvRect roi);


};

#endif // FINDOBJECT_H




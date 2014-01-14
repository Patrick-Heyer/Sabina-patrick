#include "matchingsurf.h"

MatchingSURF::MatchingSURF()
{
}

int MatchingSURF::findNaiveNearestNeighbor(const float* image1Descriptor, const CvSURFPoint* image1KeyPoint, CvSeq* image2Descriptors, CvSeq* image2KeyPoints)
{
    int descriptorsCount = (int)(image2Descriptors->elem_size/sizeof(float));
    double minSquaredDistance = std::numeric_limits<double>::max();
    double lastMinSquaredDistance = std::numeric_limits<double>::max();

    int neighbor;
    for (int i = 0; i < image2Descriptors->total; i++) {
        const CvSURFPoint* image2KeyPoint = (const CvSURFPoint*) cvGetSeqElem(image2KeyPoints, i);
        const float* image2Descriptor = (const float*) cvGetSeqElem(image2Descriptors, i);

        if (image1KeyPoint->laplacian != image2KeyPoint->laplacian)
            continue; // Don't worry about key points unless laplacian signs are equal

        double squaredDistance = this->compareSURFDescriptors(image1Descriptor, image2Descriptor, descriptorsCount, lastMinSquaredDistance);

        if (squaredDistance < minSquaredDistance) {
            neighbor = i;
            lastMinSquaredDistance = minSquaredDistance;
            minSquaredDistance = squaredDistance;
        } else if (squaredDistance < lastMinSquaredDistance) {
            lastMinSquaredDistance = squaredDistance;
        }
    }

    //change 1
    if (minSquaredDistance < 0.6 * lastMinSquaredDistance)
        return neighbor;

    return NO_NEIGHBOR;

}


int MatchingSURF::findNaiveNearestNeighbor_1(const float *image1Descriptor, const CvSURFPoint *image1KeyPoint, CvSeq *image2Descriptors, CvSeq *image2KeyPoints, vector<int> indices2)
{

    int descriptorsCount = (int)(image2Descriptors->elem_size/sizeof(float));
    double minSquaredDistance = std::numeric_limits<double>::max();
    double lastMinSquaredDistance = std::numeric_limits<double>::max();

    int neighbor;
    int totalkp2=indices2.size();
    for (int i = 0; i < totalkp2; i++) {
        const CvSURFPoint* image2KeyPoint = (const CvSURFPoint*) cvGetSeqElem(image2KeyPoints, indices2.at(i));
        const float* image2Descriptor = (const float*) cvGetSeqElem(image2Descriptors, indices2.at(i));

        if (image1KeyPoint->laplacian != image2KeyPoint->laplacian)
            continue; // Don't worry about key points unless laplacian signs are equal

        double squaredDistance = this->compareSURFDescriptors(image1Descriptor, image2Descriptor, descriptorsCount, lastMinSquaredDistance);

        if (squaredDistance < minSquaredDistance) {
            neighbor = indices2.at(i);
            lastMinSquaredDistance = minSquaredDistance;
            minSquaredDistance = squaredDistance;
        } else if (squaredDistance < lastMinSquaredDistance) {
            lastMinSquaredDistance = squaredDistance;
        }
    }

    //change 1
    if (minSquaredDistance < 0.6 * lastMinSquaredDistance)
        return neighbor;

    return NO_NEIGHBOR;

}


/*
 double MatchingSURF::compareSURFDescriptors(const float* image1Descriptor, const float* image2Descriptor, int descriptorsCount, float lastMinSquaredDistance)
 {
 double totalCost = 0;

 for (int i = 0; i < descriptorsCount; i += 4) {
 QVector4D descriptor1(image1Descriptor[i+0], image1Descriptor[i+1], image1Descriptor[i+2], image1Descriptor[i+3]);
 QVector4D descriptor2(image2Descriptor[i+0], image2Descriptor[i+1], image2Descriptor[i+2], image2Descriptor[i+3]);
 totalCost += (descriptor2 - descriptor1).lengthSquared();
 if (totalCost >lastMinSquaredDistance )
 break;
 }

 return totalCost;
 }
 */


vector<vector<CvPoint2D32f> > MatchingSURF::findMatchingKeyPoints_1(CvSeq *image1KeyPoints, CvSeq *image1Descriptors, CvSeq *image2KeyPoints, CvSeq *image2Descriptors, vector<int> &indicesKeyPoints2, vector<int> &indices1, vector<int> &indices2)
{
    indices1.clear();
    indices2.clear();
    // Find matching keypoints in both images
    vector<vector<CvPoint2D32f> > keyPointMatches;
    keyPointMatches.push_back(vector<CvPoint2D32f>());
    keyPointMatches.push_back(vector<CvPoint2D32f>());
    for (int i = 0; i < image1Descriptors->total; i++) {
        const CvSURFPoint* image1KeyPoint = (const CvSURFPoint*) cvGetSeqElem(image1KeyPoints, i);
        const float* image1Descriptor =  (const float*) cvGetSeqElem(image1Descriptors, i);
        int nearestNeighbor =
            this->findNaiveNearestNeighbor_1(
                image1Descriptor,
                image1KeyPoint,
                image2Descriptors,
                image2KeyPoints,indicesKeyPoints2
            );

        if (nearestNeighbor == NO_NEIGHBOR) {
            continue;
        }

        keyPointMatches[0].push_back(((CvSURFPoint*) cvGetSeqElem(image1KeyPoints, i))->pt);
        keyPointMatches[1].push_back(((CvSURFPoint*) cvGetSeqElem(image2KeyPoints, nearestNeighbor))->pt);
        indices1.push_back(i);
        indices2.push_back(nearestNeighbor);

    }

    return keyPointMatches;
}



vector<vector<CvPoint2D32f> > MatchingSURF::findMatchingKeyPoints(CvSeq* image1KeyPoints, CvSeq* image1Descriptors, CvSeq* image2KeyPoints, CvSeq* image2Descriptors, vector<int>& indices1,vector<int>& indices2 )
{
    indices1.clear();
    indices2.clear();
    // Find matching keypoints in both images
    vector<vector<CvPoint2D32f> > keyPointMatches;
    keyPointMatches.push_back(vector<CvPoint2D32f>());
    keyPointMatches.push_back(vector<CvPoint2D32f>());
    for (int i = 0; i < image1Descriptors->total; i++) {
        const CvSURFPoint* image1KeyPoint = (const CvSURFPoint*) cvGetSeqElem(image1KeyPoints, i);
        const float* image1Descriptor =  (const float*) cvGetSeqElem(image1Descriptors, i);
        int nearestNeighbor =
            this->findNaiveNearestNeighbor(
                image1Descriptor,
                image1KeyPoint,
                image2Descriptors,
                image2KeyPoints
            );

        if (nearestNeighbor == NO_NEIGHBOR) {
            continue;
        }

        keyPointMatches[0].push_back(((CvSURFPoint*) cvGetSeqElem(image1KeyPoints, i))->pt);
        keyPointMatches[1].push_back(((CvSURFPoint*) cvGetSeqElem(image2KeyPoints, nearestNeighbor))->pt);
        indices1.push_back(i);
        indices2.push_back(nearestNeighbor);

    }

    return keyPointMatches;
}





vector<vector<CvPoint2D32f> > MatchingSURF::findMatchingKeyPoints2(CvSeq *image1KeyPoints, CvSeq *image1Descriptors, CvSeq *image2KeyPoints, CvSeq *image2Descriptors, vector<int> &indices1, vector<int> &indices2)
{
    indices1.clear();
    indices2.clear();
    // Find matching keypoints in both images
    vector<vector<CvPoint2D32f> > keyPointMatches;
    keyPointMatches.push_back(vector<CvPoint2D32f>());
    keyPointMatches.push_back(vector<CvPoint2D32f>());

    int n1=image1Descriptors->total;
    int  n2=image2Descriptors->total;
    int descriptorsCount = (int)(image2Descriptors->elem_size/sizeof(float));
    double** matriz=new double*[n1];

    double max=0;
    for (int i = 0; i < n1; i++) {
        matriz[i]=new double[n2];
        const CvSURFPoint* image1KeyPoint = (const CvSURFPoint*) cvGetSeqElem(image1KeyPoints, i);

        const float* image1Descriptor =  (const float*) cvGetSeqElem(image1Descriptors, i);
        for (int j = 0; j < n2; j++) {
            const CvSURFPoint* image2KeyPoint = (const CvSURFPoint*) cvGetSeqElem(image2KeyPoints, j);
            const float* image2Descriptor =  (const float*) cvGetSeqElem(image2Descriptors, j);

            if (image1KeyPoint->laplacian != image2KeyPoint->laplacian)
                matriz[i][j]=10000;
            else {
                double totalCost=compareSURFDescriptors(image1Descriptor,image2Descriptor,descriptorsCount);
                matriz[i][j]=totalCost;
                if(totalCost>max)
                    max=totalCost;
            }//if
        }//j
    }//i

    //Formar matriz de similitud
    for (int i = 0; i < n1; i++)
        for (int j = 1; j < n2; j++)
            if(matriz[i][j]==10000)
                matriz[i][j]=0;
            else
                matriz[i][j]=max/(max+matriz[i][j]);
    //encontrar las mayores similitudes en filas y columnas
    //  Mayor por filas
    for (int i = 0; i < n1; i++) {
        int maximo=0;
        for (int j = 1; j < n2; j++)
            if(matriz[i][j]>matriz[i][maximo])
                maximo=j;
        for (int j = 0; j < n2; j++)
            if(maximo!=j)
                matriz[i][j]=0;

    }

    //  Mayor por columnas
    for (int j = 0; j < n2; j++) {
        int maximo=0;
        for (int i = 1; i < n1; i++)
            if(matriz[i][j]>matriz[maximo][j])
                maximo=i;
        for (int i = 1; i < n1; i++)
            if(maximo!=i)
                matriz[i][j]=0;

    }

    //Encontrar valores distintos de 0 o mayores a un umbral


    for (int i = 0; i < n1; i++)
        for (int j = 1; j < n2; j++)
            if(matriz[i][j]>0) {

                keyPointMatches[0].push_back(((CvSURFPoint*) cvGetSeqElem(image1KeyPoints, i))->pt);
                keyPointMatches[1].push_back(((CvSURFPoint*) cvGetSeqElem(image2KeyPoints, j))->pt);
                indices1.push_back(i);
                indices2.push_back(j);
            }


    for (int i = 0; i < n1; i++)
        delete[] matriz[i];


    return keyPointMatches;
}

vector<vector<CvPoint2D32f> > MatchingSURF::findMatchingKeyPoints2_1(CvSeq *image1KeyPoints, CvSeq *image1Descriptors, CvSeq *image2KeyPoints, CvSeq *image2Descriptors, vector<int> &indicesKeyPoints2, vector<int> &indices1, vector<int> &indices2) {
    indices1.clear();
    indices2.clear();
    // Find matching keypoints in both images
    vector<vector<CvPoint2D32f> > keyPointMatches;
    keyPointMatches.push_back(vector<CvPoint2D32f>());
    keyPointMatches.push_back(vector<CvPoint2D32f>());

    int n1=image1Descriptors->total;
    int  n2=indicesKeyPoints2.size();
    int descriptorsCount = (int)(image2Descriptors->elem_size/sizeof(float));
    double** matriz=new double*[n1];

    double max=0;
    for (int i = 0; i < n1; i++) {
        matriz[i]=new double[n2];
        const CvSURFPoint* image1KeyPoint = (const CvSURFPoint*) cvGetSeqElem(image1KeyPoints, i);

        const float* image1Descriptor =  (const float*) cvGetSeqElem(image1Descriptors, i);
        for (int j = 0; j < n2; j++) {
            const CvSURFPoint* image2KeyPoint = (const CvSURFPoint*) cvGetSeqElem(image2KeyPoints, indicesKeyPoints2.at(j));
            const float* image2Descriptor =  (const float*) cvGetSeqElem(image2Descriptors, indicesKeyPoints2.at(j));

            if (image1KeyPoint->laplacian != image2KeyPoint->laplacian)
                matriz[i][j]=10000;
            else {
                double totalCost=compareSURFDescriptors(image1Descriptor,image2Descriptor,descriptorsCount);
                matriz[i][j]=totalCost;
                if(totalCost>max)
                    max=totalCost;
            }//if
        }//j
    }//i

    //Formar matriz de similitud
    for (int i = 0; i < n1; i++)
        for (int j = 1; j < n2; j++)
            if(matriz[i][j]==10000)
                matriz[i][j]=0;
            else
                matriz[i][j]=max/(max+matriz[i][j]);
    //encontrar las mayores similitudes en filas y columnas
    //  Mayor por filas
    for (int i = 0; i < n1; i++) {
        int maximo=0;
        for (int j = 1; j < n2; j++)
            if(matriz[i][j]>matriz[i][maximo])
                maximo=j;
        for (int j = 0; j < n2; j++)
            if(maximo!=j)
                matriz[i][j]=0;

    }

    //  Mayor por columnas
    for (int j = 0; j < n2; j++) {
        int maximo=0;
        for (int i = 1; i < n1; i++)
            if(matriz[i][j]>matriz[maximo][j])
                maximo=i;
        for (int i = 1; i < n1; i++)
            if(maximo!=i)
                matriz[i][j]=0;

    }

    //Encontrar valores distintos de 0 o mayores a un umbral


    for (int i = 0; i < n1; i++)
        for (int j = 1; j < n2; j++)
            if(matriz[i][j]>0) {

                keyPointMatches[0].push_back(((CvSURFPoint*) cvGetSeqElem(image1KeyPoints, i))->pt);
                keyPointMatches[1].push_back(((CvSURFPoint*) cvGetSeqElem(image2KeyPoints, indicesKeyPoints2.at(j)))->pt);
                indices1.push_back(i);
                indices2.push_back(indicesKeyPoints2.at(j));
            }


    for (int i = 0; i < n1; i++)
        delete[] matriz[i];


    return keyPointMatches;
}


double MatchingSURF::compareSURFDescriptors(const float *image1Descriptor, const float *image2Descriptor, int descriptorsCount)
{

    double totalCost = 0;

    for (int i = 0; i < descriptorsCount; i += 4) {
        double sum=0;
        for (int j=0; j<4; j++) {
            sum+=(image1Descriptor[i+j]-image2Descriptor[i+j])*(image1Descriptor[i+j]-image2Descriptor[i+j]);
        }
        // sum=sqrt(sum);
        totalCost+=sum;
    }

    return (totalCost/double(descriptorsCount));

}

double MatchingSURF::compareSURFDescriptors(const float* image1Descriptor, const float* image2Descriptor, int descriptorsCount, float lastMinSquaredDistance)
{
    double totalCost = 0;

    for (int i = 0; i < descriptorsCount; i += 4) {
        double sum=0;
        for (int j=0; j<4; j++) {
            sum+=(image1Descriptor[i+j]-image2Descriptor[i+j])*(image1Descriptor[i+j]-image2Descriptor[i+j]);
        }
        //       sum=sqrt(sum);
        //        QVector4D descriptor1(image1Descriptor[i+0], image1Descriptor[i+1], image1Descriptor[i+2], image1Descriptor[i+3]);
        //      QVector4D descriptor2(image2Descriptor[i+0], image2Descriptor[i+1], image2Descriptor[i+2], image2Descriptor[i+3]);
        //    totalCost += (descriptor2 - descriptor1).lengthSquared();
        totalCost+=sum;
        if (totalCost >lastMinSquaredDistance )
            break;
    }

    return totalCost;
}

double MatchingSURF::findHomography(CvSeq *image1KeyPoints, CvSeq *image1Descriptors, CvSeq *image2KeyPoints, CvSeq *image2Descriptors) {

    MatchingSURF ms;
    vector<int> ind1, ind2;
    vector<vector<CvPoint2D32f> > matchingKeyPoints = ms.findMatchingKeyPoints2(
                image1KeyPoints,
                image1Descriptors,
                image2KeyPoints,
                image2Descriptors,
                ind1,ind2);

    int n=ind1.size();

    //   qDebug()<<"surf"<<n<<"      d1"<<image1Descriptors->total<<"    d2"<<image2Descriptors->total;
    if(n<20)
        return 0;

    //  qDebug() << "     * Finding homography whit opencv: " ;
    CvMat image1Points = cvMat(1, n, CV_32FC2, &ind1[0] );
    CvMat image2Points = cvMat(1, n, CV_32FC2, &ind2[0] );


    double h[9];
    CvMat H = cvMat(3, 3, CV_64F, h);
    if(cvFindHomography(&image1Points, &image2Points, &H,CV_RANSAC)) {
        //   return ImageSimilarity().keyPoints(ind1,ind2,image1Descriptors, image2Descriptors);

        float** homography=new float*[3];
        for(int k=0; k<3; k++)
            homography[k]=new float[3];

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (j == 2 && i != 2) {
                    homography[i][j] = h[i * 3 + j] ;
                } else if (i == 2 && j < 2) {
                    homography[i][j] = h[i * 3 + j] ;
                } else {
                    homography[i][j] = h[i * 3 + j];
                }
            }
        }
        double det=homography[0][0]*homography[1][1]*homography[2][2]+ homography[0][1]*homography[1][2]*homography[2][0]+ homography[0][2]*homography[1][0]*homography[2][1]-
                   homography[2][0]*homography[1][1]*homography[0][2]-homography[0][0]*homography[2][1]*homography[1][2]-homography[1][0]*homography[0][1]*homography[2][2];


        for(int k=0; k<3; k++)
            delete[] homography[k];
        delete[] homography;

        int n=4;
        if(det>=1/double(n) && det<=n)
            return ImageSimilarity().keyPoints(ind1,ind2,image1Descriptors, image2Descriptors);
        else return 0;


    }
    else
        return 0;



}


double MatchingSURF::findHomography(CvSeq *image1KeyPoints, CvSeq *image1Descriptors, CvSeq *image2KeyPoints, CvSeq *image2Descriptors, vector<int> &indicesKeyPoints2)
{

    MatchingSURF ms;
    vector<int> ind1, ind2;
    /* vector<vector<CvPoint2D32f> > matchingKeyPoints = ms.findMatchingKeyPoints2_1(
     image1KeyPoints,
     image1Descriptors,
     image2KeyPoints,
     image2Descriptors,indicesKeyPoints2,
     ind1,ind2);
     */

    vector<vector<CvPoint2D32f> > matchingKeyPoints = ms.findMatchingKeyPoints_1(
                image1KeyPoints,
                image1Descriptors,
                image2KeyPoints,
                image2Descriptors,indicesKeyPoints2,
                ind1,ind2);
    int n=ind1.size();

    //       qDebug()<<"surf n1"<<n<<"      d1 n2"<<ind2.size()<<"  "<<image1Descriptors->total<<"    d2"<<image2Descriptors->total;
    if(n<4)
        return 0;

    //  qDebug() << "     * Finding homography whit opencv: " ;
    vector<CvPoint2D32f> pt1, pt2;

    CvMat image1Points;// = cvMat(1, n, CV_32FC2, &matchingKeyPoints[0] );//changes

    CvMat image2Points;// = cvMat(1, n, CV_32FC2, &matchingKeyPoints[1] );//changes

    //NUEVO CAMBIO 1
    //Observar cuantas asignaciones distintas se realizan a la imagen 2(scena)
    int* countAsignation=new int[image2KeyPoints->total];
    for(int i = 0; i <image2KeyPoints->total ; i++)countAsignation[i]=0;
    for(int i = 0; i < n ; i++ )
    {
        pt1.push_back(((CvSURFPoint*)cvGetSeqElem(image1KeyPoints,ind1.at(i)))->pt);
        pt2.push_back(((CvSURFPoint*)cvGetSeqElem(image2KeyPoints,ind2.at(i)))->pt);
        countAsignation[ind2.at(i)]++;
    }
    //obtener maximo de asignaiciones
    int max=0;
    for(int i = 0; i <image2KeyPoints->total ; i++)
        if(countAsignation[i]>max)
            max=countAsignation[i];

    if(max>0.4*n) {
        return 0;
        //qDebug()<<"Ocurrio correspondencia mala...";
    }
    image1Points = cvMat(1, n, CV_32FC2, &pt1[0] );
    image2Points = cvMat(1, n, CV_32FC2, &pt2[0] );



    double h[9];
    CvMat H = cvMat(3, 3, CV_64F, h);
    if(cvFindHomography(&image1Points, &image2Points, &H,CV_RANSAC)) {
        //   return ImageSimilarity().keyPoints(ind1,ind2,image1Descriptors, image2Descriptors);

        float** homography=new float*[3];
        for(int k=0; k<3; k++)
            homography[k]=new float[3];

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (j == 2 && i != 2) {
                    homography[i][j] = h[i * 3 + j] ;
                } else if (i == 2 && j < 2) {
                    homography[i][j] = h[i * 3 + j] ;
                } else {
                    homography[i][j] = h[i * 3 + j];
                }
            }
        }
        double det=homography[0][0]*homography[1][1]*homography[2][2]+ homography[0][1]*homography[1][2]*homography[2][0]+ homography[0][2]*homography[1][0]*homography[2][1]-
                   homography[2][0]*homography[1][1]*homography[0][2]-homography[0][0]*homography[2][1]*homography[1][2]-homography[1][0]*homography[0][1]*homography[2][2];


        for(int k=0; k<3; k++)
            delete[] homography[k];
        delete[] homography;

        int n=7;
        if(det>=1/double(n) && det<=n)
            return ImageSimilarity().keyPoints(ind1,ind2,image1Descriptors, image2Descriptors);
        else return 0;


    }
    else
        return 0;



}


void MatchingSURF::pointsInRect(CvSeq *scenetKeyPointComplete, CvSeq *sceneDescriptorsComplete, vector<int>&indices, CvRect rect) {
    //recorrer todos los puntos surf de la escena

    int n1=sceneDescriptorsComplete->total;

    for(int i=0; i<n1; i++) {
        //Obtener coordenadas del punto
        const CvSURFPoint* image1KeyPoint = (const CvSURFPoint*) cvGetSeqElem(scenetKeyPointComplete, i);
        int x=image1KeyPoint->pt.x;
        int y=image1KeyPoint->pt.y;

        int x1=rect.x;
        int y1=rect.y;

        int x2=rect.x+rect.width;
        int y2=rect.y+rect.height;
        if(x>=x1 && x<=x2 & y>=y1 && y<=y2)
            indices.push_back(i);




    }

}

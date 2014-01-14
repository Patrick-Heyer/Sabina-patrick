/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  <copyright holder> <email>

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


#include "surfrecognizer.h"

SurfRecognizer::SurfRecognizer()
{
    displaying = true;
    storage = cvCreateMemStorage(0);
    windowOpened = false;

    objectKeypoints = 0;
    objectDescriptors = 0;

    params = cvSURFParams(500, 1);

}

SurfRecognizer::~SurfRecognizer()
{
    cvDestroyWindow("Object");
    cvDestroyWindow("Object SURF");
    cvDestroyWindow("Object Correspond");
}


void SurfRecognizer::setObjectName(string name)
{
    objectName.append(name);
}

void SurfRecognizer::setRecognitionTreshold(int value)
{
    recTreshold = value;
}


bool SurfRecognizer::loadObjectFile()
{
    object = cvLoadImage( objectFileName.c_str(), CV_LOAD_IMAGE_GRAYSCALE );

    if( !object )
    {
        cout <<  "Can not load " <<  objectFileName.c_str() << endl ;
        exit(-1);
    }

    cout << object->depth << endl;
    cout << object->colorModel  << endl;
    cout << object->nChannels << endl;
    cout << object->colorModel << endl;

    cvExtractSURF( object, 0, &objectKeypoints, &objectDescriptors, storage, params );

// src_corners[4] = {{0,0}, {object->width,0}, {object->width, object->height}, {0, object->height}};

    src_corners[0].x = 0;
    src_corners[0].y = 0;
    src_corners[1].x = object->width;
    src_corners[1].y = 0;
    src_corners[2].x = object->width;
    src_corners[2].y = object->height;
    src_corners[3].x = 0;
    src_corners[3].y = object->height;

    //CvPoint dst_corners[4];

    if(displaying)
        printf("Object Descriptors: %d\n", objectDescriptors->total);

    //cvNamedWindow("Object", 1);
    //cvShowImage( "Object", object );

    //cvWaitKey(0);

    return true;
}


void SurfRecognizer::setDisplaying(bool value)
{
    displaying = value;
}


void SurfRecognizer::displayObject()
{
    cvNamedWindow("Object", 1);

    IplImage* object_color = cvCreateImage(cvGetSize(object), 8, 3);
    cvCvtColor( object, object_color, CV_GRAY2BGR );

    CvSeq *objectKeypoints = 0, *objectDescriptors = 0;
    int i;
    CvSURFParams params = cvSURFParams(500, 1);

    /// Extraer puntos SURF
    double tt = (double)cvGetTickCount();
    cvExtractSURF( object, 0, &objectKeypoints, &objectDescriptors, storage, params );

    if(displaying)
        printf("Object Descriptors: %d\n", objectDescriptors->total);


    // Displaying object points.
    for( i = 0; i < objectKeypoints->total; i++ )
    {
        CvSURFPoint* r = (CvSURFPoint*)cvGetSeqElem( objectKeypoints, i );
        CvPoint center;
        int radius;
        center.x = cvRound(r->pt.x);
        center.y = cvRound(r->pt.y);
        radius = cvRound(r->size*1.2/9.*2);
        cvCircle( object_color, center, radius, colors[0], 1, 8, 0 );
    }

    //if(displaying)
    cvShowImage( "Object", object_color );

}


bool SurfRecognizer::recongnizeObjectInImage(IplImage* imageWithObject)
{
    if( !imageWithObject )
    {
        cout << "No image received" << endl;
        return false;
    }

    if(displaying && (!windowOpened)) {
        cvNamedWindow("Object Correspond", 1);
        windowOpened = true;
    }

    IplImage* image = cvCreateImage( cvSize( imageWithObject->width, imageWithObject->height ), IPL_DEPTH_8U, 1 );;
    cvCvtColor(imageWithObject, image, CV_RGB2GRAY);

    CvSeq *imageKeypoints = 0, *imageDescriptors = 0;
    int i;

    /// Extraer puntos SURF
    double tt = (double)cvGetTickCount();

    cvExtractSURF( image, 0, &imageKeypoints, &imageDescriptors, storage, params );

    if(displaying)
        printf("Image Descriptors: %d\n", imageDescriptors->total);

    tt = (double)cvGetTickCount() - tt;

    if(displaying)
        printf( "Extraction time = %gms\n", tt/(cvGetTickFrequency()*1000.));

    //CvPoint src_corners[4] = {{0,0}, {object->width,0}, {object->width, object->height}, {0, object->height}};
    //CvPoint dst_corners[4];

    IplImage* correspond = cvCreateImage( cvSize(image->width, object->height+image->height), 8, 1 );
    cvSetImageROI( correspond, cvRect( 0, 0, object->width, object->height ) );
    cvCopy( object, correspond );
    cvSetImageROI( correspond, cvRect( 0, object->height, correspond->width, correspond->height ) );
    cvCopy( image, correspond );
    cvResetImageROI( correspond );


#ifdef USE_FLANN
    printf("Using approximate nearest neighbor search\n");
#endif

    int n;
    n = locatePlanarObject( objectKeypoints, objectDescriptors, imageKeypoints,
                            imageDescriptors, src_corners, dst_corners );

//     if( locatePlanarObject( objectKeypoints, objectDescriptors, imageKeypoints,
// 	  imageDescriptors, src_corners, dst_corners ))
    if(n)
    {
        if( n >= recTreshold) {
            objectFounded = true;
            //if(displaying)
            cout << "Found!!!" << endl;
        } else {
            objectFounded = false;
            if(displaying)
                cout << "Not found" << endl;
        }

        for( i = 0; i < 4; i++ )
        {
            CvPoint r1 = dst_corners[i%4];
            CvPoint r2 = dst_corners[(i+1)%4];
            cvLine( correspond, cvPoint(r1.x, r1.y+object->height ),
                    cvPoint(r2.x, r2.y+object->height ), colors[8] );
        }
    }
    else {
        objectFounded = false;

        if(displaying)
            cout << "Not found" << endl;
    }


    if(displaying) {
        vector<int> ptpairs;
#ifdef USE_FLANN
        flannFindPairs( objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs );
#else
        findPairs( objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs );
#endif

        for( i = 0; i < (int)ptpairs.size(); i += 2 )
        {
            CvSURFPoint* r1 = (CvSURFPoint*)cvGetSeqElem( objectKeypoints, ptpairs[i] );
            CvSURFPoint* r2 = (CvSURFPoint*)cvGetSeqElem( imageKeypoints, ptpairs[i+1] );
            cvLine( correspond, cvPointFrom32f(r1->pt),
                    cvPoint(cvRound(r2->pt.x), cvRound(r2->pt.y+object->height)), colors[8] );
        }
    }

    if(displaying)
        cvShowImage( "Object Correspond", correspond );


    //cvWaitKey(0);

    if ( (cvWaitKey(10) & 255) == 27 ) return false;

    return objectFounded;
}

float euclideanDist(CvPoint& p, CvPoint& q) {
    CvPoint diff;
    diff.x=p.x-q.x;
    diff.y=p.y-q.y;
    return cv::sqrt(diff.x*diff.x + diff.y*diff.y);
}


bool SurfRecognizer::recongnizeObjectInImage(IplImage* imageWithObject, int &X, int &Y)
{
    if( !imageWithObject )
    {
        cout << "No image received" << endl;
        return false;
    }

    if(displaying && (!windowOpened)) {
        cvNamedWindow("Object Correspond", 1);
        windowOpened = true;
    }

    IplImage* image = cvCreateImage( cvSize( imageWithObject->width, imageWithObject->height ), IPL_DEPTH_8U, 1 );;
    cvCvtColor(imageWithObject, image, CV_RGB2GRAY);

    CvSeq *imageKeypoints = 0, *imageDescriptors = 0;
    int i;

    /// Extraer puntos SURF
    double tt = (double)cvGetTickCount();

    cvExtractSURF( image, 0, &imageKeypoints, &imageDescriptors, storage, params );

    if(displaying)
        printf("Image Descriptors: %d\n", imageDescriptors->total);

    tt = (double)cvGetTickCount() - tt;

    if(displaying)
        printf( "Extraction time = %gms\n", tt/(cvGetTickFrequency()*1000.));

    //CvPoint src_corners[4] = {{0,0}, {object->width,0}, {object->width, object->height}, {0, object->height}};
    //CvPoint dst_corners[4];

    IplImage* correspond = cvCreateImage( cvSize(image->width, object->height+image->height), 8, 1 );
    cvSetImageROI( correspond, cvRect( 0, 0, object->width, object->height ) );
    cvCopy( object, correspond );
    cvSetImageROI( correspond, cvRect( 0, object->height, correspond->width, correspond->height ) );
    cvCopy( image, correspond );
    cvResetImageROI( correspond );


#ifdef USE_FLANN
    printf("Using approximate nearest neighbor search\n");
#endif

    int n;
    n = locatePlanarObject( objectKeypoints, objectDescriptors, imageKeypoints,
                            imageDescriptors, src_corners, dst_corners );

//     if( locatePlanarObject( objectKeypoints, objectDescriptors, imageKeypoints,
// 	  imageDescriptors, src_corners, dst_corners ))
    if(n)
    {
        if( n >= recTreshold) {
            objectFounded = true;
	    

	  CvPoint center;
            //if(displaying)
	  if(((dst_corners[1].x-dst_corners[0].x)/2) <50 && ((dst_corners[2].y-dst_corners[0].y)/2) <50)
	  {
	    X=dst_corners[0].x+((dst_corners[1].x-dst_corners[0].x)/2);
	    Y=dst_corners[0].y+((dst_corners[2].y-dst_corners[0].y)/2);
	  }
	  else
	  {
	    X=dst_corners[0].x+(50);
	    Y=dst_corners[0].y+(50);	    
	  }
	    	    
	    center.x=X;
            center.y=Y+object->height;
            cvCircle(correspond, center, 5, CV_RGB(0, 255, 0), -1, 8, 0);
	    cvSaveImage("../data/encontre.png",correspond);
            cout << "Found!!!" << endl;
        } else {
            objectFounded = false;
            if(displaying)
                cout << "Not found" << endl;
        }

        for( i = 0; i < 4; i++ )
        {
            CvPoint r1 = dst_corners[i%4];
            CvPoint r2 = dst_corners[(i+1)%4];
            cvLine( correspond, cvPoint(r1.x, r1.y+object->height ),
                    cvPoint(r2.x, r2.y+object->height ), colors[8] );
        }
    }
    else {
        objectFounded = false;

        if(displaying)
            cout << "Not found" << endl;
    }


    if(displaying) {
        vector<int> ptpairs;
#ifdef USE_FLANN
        flannFindPairs( objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs );
#else
        findPairs( objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs );
#endif

        for( i = 0; i < (int)ptpairs.size(); i += 2 )
        {
            CvSURFPoint* r1 = (CvSURFPoint*)cvGetSeqElem( objectKeypoints, ptpairs[i] );
            CvSURFPoint* r2 = (CvSURFPoint*)cvGetSeqElem( imageKeypoints, ptpairs[i+1] );
            cvLine( correspond, cvPointFrom32f(r1->pt),
                    cvPoint(cvRound(r2->pt.x), cvRound(r2->pt.y+object->height)), colors[8] );
	    
	     	   
        }
    }

    if(displaying)
        cvShowImage( "Object Correspond", correspond );


    //cvWaitKey(0);
    
    cvReleaseImage(&correspond);
    cvReleaseImage(&image);

    
    if ( (cvWaitKey(10) & 255) == 27 ) return false;

    return objectFounded;
}





void SurfRecognizer::setObjectFileName(string filename)
{
    objectFileName = filename;
}

string SurfRecognizer::getObjectFileName()
{
    return objectFileName;
}

double SurfRecognizer::compareSURFDescriptors(const float* d1, const float* d2, double best, int length)
{
    double total_cost = 0;
    assert( length % 4 == 0 );
    for( int i = 0; i < length; i += 4 )
    {
        double t0 = d1[i] - d2[i];
        double t1 = d1[i+1] - d2[i+1];
        double t2 = d1[i+2] - d2[i+2];
        double t3 = d1[i+3] - d2[i+3];
        total_cost += t0*t0 + t1*t1 + t2*t2 + t3*t3;
        if( total_cost > best )
            break;
    }
    return total_cost;
}

int SurfRecognizer::naiveNearestNeighbor(const float* vec, int laplacian, const CvSeq* model_keypoints, const CvSeq* model_descriptors)
{
    int length = (int)(model_descriptors->elem_size/sizeof(float));
    int i, neighbor = -1;
    double d, dist1 = 1e6, dist2 = 1e6;
    CvSeqReader reader, kreader;
    cvStartReadSeq( model_keypoints, &kreader, 0 );
    cvStartReadSeq( model_descriptors, &reader, 0 );

    for( i = 0; i < model_descriptors->total; i++ )
    {
        const CvSURFPoint* kp = (const CvSURFPoint*)kreader.ptr;
        const float* mvec = (const float*)reader.ptr;
        CV_NEXT_SEQ_ELEM( kreader.seq->elem_size, kreader );
        CV_NEXT_SEQ_ELEM( reader.seq->elem_size, reader );
        if( laplacian != kp->laplacian )
            continue;
        d = compareSURFDescriptors( vec, mvec, dist2, length );
        if( d < dist1 )
        {
            dist2 = dist1;
            dist1 = d;
            neighbor = i;
        }
        else if ( d < dist2 )
            dist2 = d;
    }
    if ( dist1 < 0.6*dist2 )
        return neighbor;
    return -1;
}


void SurfRecognizer::findPairs(const CvSeq* objectKeypoints, const CvSeq* objectDescriptors, const CvSeq* imageKeypoints, const CvSeq* imageDescriptors, std::vector< int >& ptpairs)
{
    int i;
    CvSeqReader reader, kreader;
    cvStartReadSeq( objectKeypoints, &kreader );
    cvStartReadSeq( objectDescriptors, &reader );
    ptpairs.clear();

    for( i = 0; i < objectDescriptors->total; i++ )
    {
        const CvSURFPoint* kp = (const CvSURFPoint*)kreader.ptr;
        const float* descriptor = (const float*)reader.ptr;
        CV_NEXT_SEQ_ELEM( kreader.seq->elem_size, kreader );
        CV_NEXT_SEQ_ELEM( reader.seq->elem_size, reader );
        int nearest_neighbor = naiveNearestNeighbor( descriptor, kp->laplacian, imageKeypoints, imageDescriptors );
        if( nearest_neighbor >= 0 )
        {
            ptpairs.push_back(i);
            ptpairs.push_back(nearest_neighbor);
        }
    }
}


void SurfRecognizer::flannFindPairs(const CvSeq* , const CvSeq* objectDescriptors, const CvSeq* , const CvSeq* imageDescriptors, std::vector< int >& ptpairs)
{
    int length = (int)(objectDescriptors->elem_size/sizeof(float));

    cv::Mat m_object(objectDescriptors->total, length, CV_32F);
    cv::Mat m_image(imageDescriptors->total, length, CV_32F);


    // copy descriptors
    CvSeqReader obj_reader;
    float* obj_ptr = m_object.ptr<float>(0);
    cvStartReadSeq( objectDescriptors, &obj_reader );
    for(int i = 0; i < objectDescriptors->total; i++ )
    {
        const float* descriptor = (const float*)obj_reader.ptr;
        CV_NEXT_SEQ_ELEM( obj_reader.seq->elem_size, obj_reader );
        memcpy(obj_ptr, descriptor, length*sizeof(float));
        obj_ptr += length;
    }
    CvSeqReader img_reader;
    float* img_ptr = m_image.ptr<float>(0);
    cvStartReadSeq( imageDescriptors, &img_reader );
    for(int i = 0; i < imageDescriptors->total; i++ )
    {
        const float* descriptor = (const float*)img_reader.ptr;
        CV_NEXT_SEQ_ELEM( img_reader.seq->elem_size, img_reader );
        memcpy(img_ptr, descriptor, length*sizeof(float));
        img_ptr += length;
    }

    // find nearest neighbors using FLANN
    cv::Mat m_indices(objectDescriptors->total, 2, CV_32S);
    cv::Mat m_dists(objectDescriptors->total, 2, CV_32F);
    cv::flann::Index flann_index(m_image, cv::flann::KDTreeIndexParams(4));  // using 4 randomized kdtrees
    flann_index.knnSearch(m_object, m_indices, m_dists, 2, cv::flann::SearchParams(64) ); // maximum number of leafs checked

    int* indices_ptr = m_indices.ptr<int>(0);
    float* dists_ptr = m_dists.ptr<float>(0);
    for (int i=0; i<m_indices.rows; ++i) {
        if (dists_ptr[2*i]<0.6*dists_ptr[2*i+1]) {
            ptpairs.push_back(i);
            ptpairs.push_back(indices_ptr[2*i]);
        }
    }
}


int SurfRecognizer::locatePlanarObject(const CvSeq* objectKeypoints, const CvSeq* objectDescriptors, const CvSeq* imageKeypoints, const CvSeq* imageDescriptors, const CvPoint src_corners[4], CvPoint dst_corners[4])
{
    double h[9];
    CvMat _h = cvMat(3, 3, CV_64F, h);
    vector<int> ptpairs;
    vector<CvPoint2D32f> pt1, pt2;
    CvMat _pt1, _pt2;
    int i, n;

// #ifdef USE_FLANN
    flannFindPairs( objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs );
// #else
//     findPairs( objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs );
// #endif

    n = ptpairs.size()/2;

    if( n < 4 )
        return 0;

    pt1.resize(n);
    pt2.resize(n);
    for( i = 0; i < n; i++ )
    {
        pt1[i] = ((CvSURFPoint*)cvGetSeqElem(objectKeypoints,ptpairs[i*2]))->pt;
        pt2[i] = ((CvSURFPoint*)cvGetSeqElem(imageKeypoints,ptpairs[i*2+1]))->pt;
    }

    _pt1 = cvMat(1, n, CV_32FC2, &pt1[0] );
    _pt2 = cvMat(1, n, CV_32FC2, &pt2[0] );
    if( !cvFindHomography( &_pt1, &_pt2, &_h, CV_RANSAC, 5 ))
        return 0;

    for( i = 0; i < 4; i++ )
    {
        double x = src_corners[i].x, y = src_corners[i].y;
        double Z = 1./(h[6]*x + h[7]*y + h[8]);
        double X = (h[0]*x + h[1]*y + h[2])*Z;
        double Y = (h[3]*x + h[4]*y + h[5])*Z;
        dst_corners[i] = cvPoint(cvRound(X), cvRound(Y));
    }

    return n;
}

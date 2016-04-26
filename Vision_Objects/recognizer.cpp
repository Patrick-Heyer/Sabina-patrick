/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  <Elias Ruiz> <elias_ruiz@inaoep.mx>

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
#include "recognizer.hpp"

//nfeatures=500, scaleFactor=1.2f, nlevels=8, edgeThreshold=16, firstLevel=0, WTA_K=2, scoreType=ORB::HARRIS_SCORE, patchSize=31)
//ORB Orb(2000, 1.2f, 2, 31, 0, 2, ORB::FAST_SCORE, 4); // Bastante estático.
// SIFT int nfeatures=0, int nOctaveLayers=3, double contrastThreshold=0.04, double edgeThreshold=10, double sigma=1.6
OrbRecognizer::OrbRecognizer()
{
    displaying = true;
    windowOpened = false;
    nndrRatio = 0.6;
    TypeMachine = 1;
    NumTrainSet = 0;
    TypeDistance = 1;
    NumMaxTrainingSet = 15;
    //objectDescriptors = new cv::Mat(100,32,CV_8U) ;
    objectDescriptors = new Mat;
    OrbMachine  = new ORB(200, 1.002f, 4, 10, 0, 2, cv::ORB::FAST_SCORE, 27);
    OrbMachine2 = new ORB(120, 1.002f, 4, 10, 0, 2, cv::ORB::FAST_SCORE, 27);
    SiftMachine = new SIFT(150, 2, 0.04, 10, 1.6);
    //hessianThreshold, nOctaves=4, nOctaveLayers=2, extended=true, upright=false
    SurfMachine = new SURF(1400.0, 2, 2, true, false);
}

OrbRecognizer::~OrbRecognizer()
{
    cvDestroyWindow("Object Correspond");
    delete OrbMachine;
    delete OrbMachine2;
    delete objectDescriptors;
    delete SiftMachine;
    delete SurfMachine;
    for(int y = 0; y < NumTrainSet; y++)
    {
        delete trainKeypoints[y];
        delete trainDescriptors[y];
    }
    delete[] trainKeypoints;
    delete[] trainDescriptors;
}

void OrbRecognizer::setObjectName(string name)        {
    objectName.append(name);
}
void OrbRecognizer::setRecognitionTreshold(int value) {
    recTreshold = value;
}
void OrbRecognizer::setThreshold(float value)		{
    nndrRatio  = value;
}
void OrbRecognizer::setDisplaying(bool value)		{
    displaying = value;
}
void OrbRecognizer::setObjectFileName(string filename) {
    objectFileName = filename;
}
void OrbRecognizer::setMachine(int value)		{
    TypeMachine = value;
}
void OrbRecognizer::setDistance(int value)            {
    TypeDistance = value;
}
void OrbRecognizer::setNumMaxTrainingSet(int value) 	{
    NumMaxTrainingSet = value;
}
string OrbRecognizer::getObjectFileName()		{
    return objectFileName;
}

bool OrbRecognizer::loadObjectFile()
{
    object      = cvLoadImage( objectFileName.c_str(), CV_LOAD_IMAGE_GRAYSCALE );
    objectColor = cvLoadImage( objectFileName.c_str(), CV_LOAD_IMAGE_COLOR );
    if(!object)
    {
        cout <<  "Can not load " <<  objectFileName.c_str() << endl ;
        exit(-1);
    }
    int x = object->width; // ancho = eje X = width
    int y = object->height; // altura = eje Y = HEIGHT.
    int N_objectPoints = 60 ; //(int) (x/14 * y/14);
    cout<<"Profundidad en Bits: "<<object->depth     << endl;
    cout<<"Color Model: "        <<object->colorModel<< endl;
    cout<<"number of Channels: " <<object->nChannels << endl;
    cout<<"dims: (fils, cols): ("<<y<<", "<<x<<")."  <<endl;
    //cout<<"Object should contain: "    <<N_objectPoints<<" Keypoints."<<endl;

    cv::Mat objectMat(object);

    //cv::ORB LocalOrb(N_objectPoints, 1.4f, 4, 0, 0, 2, cv::ORB::HARRIS_SCORE, 16);
    switch(TypeMachine)
    {
    case 1:
        OrbMachine->detect(objectMat, objectKeypoints);
        break;
    case 2:
        SurfMachine->detect(objectMat, objectKeypoints);
        break;
    case 3:
        SiftMachine->detect(objectMat, objectKeypoints);
        break;
    default:
        cout<<"No hay máquina aplicable"<<endl;
        //LocalOrb.detect(objectMat, objectKeypoints);
    }


    switch(TypeMachine)
    {
    case 1:
        OrbMachine->compute(objectMat, objectKeypoints, *objectDescriptors);
        break;
    case 2:
        SurfMachine->compute(objectMat, objectKeypoints, *objectDescriptors);
        break;
    case 3:
        SiftMachine->compute(objectMat, objectKeypoints, *objectDescriptors);
        break;
    default:
        cout<<"No hay máquina aplicable"<<endl;
        //LocalOrb.compute(objectMat, objectKeypoints, *objectDescriptors);

    }

    cout<<"Number of Keypoints: "<<objectKeypoints.size()<<endl;
    ulong NK = objectKeypoints.size();

    // Drawing Keypoints.
    for(int i = 0; i < NK; i++ )
    {
        cvCircle(objectColor, cvPoint(cvRound(objectKeypoints[i].pt.x) , cvRound(objectKeypoints[i].pt.y)), 3, colors[3]);
    }
    return true;
}

bool OrbRecognizer::recongnizeObjectInImage(IplImage* imageWithObject, int &X, int &Y)
{
    bool ORRecognizer = false;
    if( !imageWithObject )
    {
        cout << "No image received" << endl;
        return false;
    }
    if(displaying && (!windowOpened)) {
        namedWindow("Object Correspond", 1);
        windowOpened = true;
    }

    vector<KeyPoint> imageKeypoints;
    Mat imageDescriptors;
    Mat imageMat(imageWithObject);
    IplImage* ImgLimpia = cvCreateImage(cvSize(imageWithObject->width, imageWithObject->height), 8, 3 );
    cvCopy(imageWithObject, ImgLimpia);


    switch(2) // Normal: TypeMachine
    {
    case 1:
        OrbMachine->detect(imageMat, imageKeypoints);
        break;
    case 2:
        SurfMachine->detect(imageMat, imageKeypoints);
        break;
    case 3:
        SiftMachine->detect(imageMat, imageKeypoints);
        break;
    default:
        cout<<"No hay máquina aplicable"<<endl;
    }

    HC HcHand(imageMat.rows, imageMat.cols);
    Mat Ventanas, ExoCentros;
    cout<< "Son: "<<imageKeypoints.size()<<" Keypoints en esta imagen"<<endl;
    Mat centers = HcHand.Execute(imageKeypoints, Ventanas, ExoCentros);


    for(int k = 0; k< Ventanas.rows; k++)
    {
        cvRectangle(imageWithObject,cvPoint(cvRound(Ventanas.at<float>(k,2)), cvRound(Ventanas.at<float>(k,0))),
                    cvPoint(cvRound(Ventanas.at<float>(k,3)), cvRound(Ventanas.at<float>(k,1))), colors[8], 1);
    }


//     for(int k = 0; k< centers.rows; k++)
//     {
//       cvCircle(imageWithObject,cvPoint(cvRound(centers.at<float>(k,1)), cvRound(centers.at<float>(k,0))), 20, colors[1],2);
//     }
//        for(int k = 0; k< ExoCentros.rows; k++)
//     {
//       cvCircle(imageWithObject,cvPoint(cvRound(ExoCentros.at<float>(k,1)), cvRound(ExoCentros.at<float>(k,0))), 25, colors[2], 3);
//     }

    ulong NobK = imageKeypoints.size();

    //Drawing KeyPoints.
    for(int i = 0; i < NobK; i++ )
        cvCircle( imageWithObject, cvPoint(cvRound(imageKeypoints[i].pt.x), cvRound(imageKeypoints[i].pt.y)), 3, colors[i % 9]);



//       switch(TypeMachine)
//       {
//       case 1:
// 	OrbMachine->compute(imageMat, imageKeypoints, imageDescriptors);
// 	break;
//       case 2:
// 	SurfMachine->compute(imageMat, imageKeypoints, imageDescriptors);
// 	break;
//       case 3:
// 	SiftMachine->compute(imageMat, imageKeypoints, imageDescriptors);
// 	break;
//       default:
// 	cout<<"No hay máquina aplicable"<<endl;
//     }

//     vector<Point2f> mpts_1, mpts_2;
//     vector<int> indexes_1, indexes_2;
//     Mat outlier_mask;
//     vector<Point2f> esq;
//     vector<DMatch> matches;
//     Mat img_matches;
//     objectFounded = flannFindPairs(objectKeypoints, *objectDescriptors, imageKeypoints, imageDescriptors,
// 				    mpts_1, mpts_2, indexes_1, indexes_2, outlier_mask, esq, matches );

    Mat ObjColor(objectColor);
    Mat ImgColor(imageWithObject);

    IplImage* correspond = cvCreateImage(cvSize(imageWithObject->width, object->height+imageWithObject->height), 8, 3 );
    cvZero       (correspond);
    cvSetImageROI(correspond, cvRect( 0, 0, object->width, object->height ) );

    cvCopy       (objectColor, correspond );
    cvSetImageROI(correspond, cvRect( 0, object->height, correspond->width, correspond->height ) );
    cvCopy       (imageWithObject, correspond );
    cvResetImageROI(correspond);

//     if(objectFounded)
//     {
//       for(int i = 0; i < (int)indexes_1.size(); i ++ )
//       {
// 	  cv::KeyPoint* r1  = &objectKeypoints[indexes_1[i]];
// 	  cv::KeyPoint* r2 =  &imageKeypoints [indexes_2[i]];
// 	  cvLine(correspond, cvPoint( r1->pt.x, r1->pt.y), cvPoint(cvRound(r2->pt.x), cvRound(r2->pt.y + object->height)), colors[3] );
// 	  X = (int) r2->pt.x;
// 	  Y = (int) r2->pt.y;
//
//       }
//       cvLine( correspond, cvPoint(esq[0].x, esq[0].y + object->height), cvPoint(esq[1].x, esq[1].y + object->height), colors[6], 4 );
//       cvLine( correspond, cvPoint(esq[1].x, esq[1].y + object->height), cvPoint(esq[2].x, esq[2].y + object->height), colors[6], 4 );
//       cvLine( correspond, cvPoint(esq[2].x, esq[2].y + object->height), cvPoint(esq[3].x, esq[3].y + object->height), colors[6], 4 );
//       cvLine( correspond, cvPoint(esq[3].x, esq[3].y + object->height), cvPoint(esq[0].x, esq[0].y + object->height), colors[6], 4 );
//     }



/////////////////////////////////////////////////////////////////////////////////////////////////7
    for(int i = 0; i< Ventanas.rows; i++) // Vamos a describir cada una de las ventanas.
    {
        int Width = Ventanas.at<float>(i, 3) - Ventanas.at<float>(i, 2);
        int Height = Ventanas.at<float>(i, 1) - Ventanas.at<float>(i, 0);
        IplImage* recorte = cvCreateImage(cvSize(Width, Height), 8, 3 );
        cvZero       (recorte);
        cvSetImageROI(ImgLimpia, cvRect( Ventanas.at<float>(i, 2), Ventanas.at<float>(i, 0), Width, Height));
        cvCopy       (ImgLimpia, recorte);
        cvResetImageROI(ImgLimpia );
        Mat Ventanita(recorte);

        switch(TypeMachine)
        {
        case 1:
            OrbMachine->detect(Ventanita, imageKeypoints);
            break;
        case 2:
            SurfMachine->detect(Ventanita, imageKeypoints);
            break;
        case 3:
            SiftMachine->detect(Ventanita, imageKeypoints);
            break;
        default:
            cout<<"No hay máquina aplicable"<<endl;
        }

        switch(TypeMachine)
        {
        case 1:
            OrbMachine->compute(Ventanita, imageKeypoints, imageDescriptors);
            break;
        case 2:
            SurfMachine->compute(Ventanita, imageKeypoints, imageDescriptors);
            break;
        case 3:
            SiftMachine->compute(Ventanita, imageKeypoints, imageDescriptors);
            break;
        default:
            cout<<"No hay máquina aplicable"<<endl;
        }
        objectFoundedV = ExecuteTesting(imageKeypoints, imageDescriptors);
//     std::vector<cv::Point2f> mpts_1, mpts_2;
//     std::vector<int> indexes_1, indexes_2 ;
//     cv::Mat outlier_mask;
//     std::vector<cv::Point2f> esq;
//     std::vector<cv::DMatch> matches;
//     cv::Mat img_matches;
//     objectFoundedV = flannFindPairs(objectKeypoints, *objectDescriptors, imageKeypoints, imageDescriptors,
// 				    mpts_1, mpts_2, indexes_1, indexes_2, outlier_mask, esq, matches );
        ORRecognizer = ORRecognizer | objectFoundedV;
        if (objectFoundedV)
        {
            cout<<"ENCONTRE EL OBJETO EN ESTA IMAGEN. VENTANA ES: "<< i<<endl;
            cvRectangle(correspond,cvPoint(cvRound(Ventanas.at<float>(i,2)), cvRound( object->height + Ventanas.at<float>(i,0))),
                        cvPoint(cvRound(Ventanas.at<float>(i,3)), cvRound( object->height + Ventanas.at<float>(i,1))), colors[7], 4);
            X = (int)((Ventanas.at<float>(i, 3) + Ventanas.at<float>(i, 2) )/2);
            Y = (int)((Ventanas.at<float>(i, 1) + Ventanas.at<float>(i, 0) )/2) ;


        }


    }
    ////////////////////////////////////////////////////////


    if(displaying) {
        cvShowImage( "Object Correspond", correspond );
        //imshow("Matches", img_matches );
        //imshow("Object Correspond", img_matches );
    }

    cvReleaseImage(&correspond);

    if ( (cvWaitKey(10) & 255) == 27 ) return false;

    return ORRecognizer;
}

bool OrbRecognizer::flannFindPairs(std::vector<cv::KeyPoint> objectKeypoints, cv::Mat objectDescriptors,
                                   std::vector<cv::KeyPoint> imageKeypoints,  cv::Mat imageDescriptors,
                                   std::vector<cv::Point2f>& mpts_1, std::vector<cv::Point2f>& mpts_2, // Used for homography
                                   std::vector<int>& indexes_1, std::vector<int>& indexes_2,
                                   cv::Mat& outlier_mask, std::vector<cv::Point2f>& scene_corners, std::vector<cv::DMatch>& matches) // Used for homography)
{
    int VALOR;

    if(1) {
        VALOR = CV_32FC1;
    } else {
        VALOR = CV_8U;
    }

    cv::Mat m_object(objectDescriptors.rows, objectDescriptors.cols, VALOR); //length es columnas
    cv::Mat m_image (imageDescriptors.rows,  imageDescriptors.cols,  VALOR);

    //cv::Mat m_object(objectDescriptors.rows, objectDescriptors.cols, CV_8U); //length es columnas
    //cv::Mat m_image (imageDescriptors.rows,  imageDescriptors.cols,  CV_8U);

    imageDescriptors.convertTo(m_image, m_image.type());
    objectDescriptors.convertTo(m_object,m_object.type());

    /// Brute Force MATCHER
    //cv::BFMatcher ObMatcher(cv::NORM_HAMMING, false );
    //ObMatcher.match(m_image, m_object, matches);

    // find nearest neighbors using FLANN
    cv::Mat m_indices(objectDescriptors.rows, 2, CV_32SC1);
    cv::Mat m_dists(objectDescriptors.rows, 2, CV_32FC1);
    //cout<<"Publicando un descriptor: "<<objectDescriptors.row(1);
    //cout<<"Publicando un descriptor: "<<imageDescriptors.row(1);
    if (m_object.type() == CV_8U) {
        cv::flann::Index flann_Index(m_image, cv::flann::LshIndexParams(12, 20, 2), cvflann::FLANN_DIST_HAMMING);  // using 4 randomized kdtrees
        flann_Index.knnSearch(m_object, m_indices, m_dists, 2, cv::flann::SearchParams() ); // maximum number of leafs checked
    } else {
        //FLANN_DIST_L2 es preferida para SURF
        cv::flann::Index flann_Index(m_image, cv::flann::KDTreeIndexParams(), cvflann::FLANN_DIST_L2);  // using 4 randomized kdtrees
        flann_Index.knnSearch(m_object, m_indices, m_dists, 2, cv::flann::SearchParams() ); // maximum number of leafs checked
    }
    //cout<<"Distancia: "<<m_dists.row(1)<<endl;
    for(unsigned int i=0; i<m_indices.rows; ++i)
    {
        if(m_dists.at<float>(i,0) <= nndrRatio * m_dists.at<float>(i,1))
            //cout<<"las distancias son:"<<m_dists.at<float>(i,0)<<", " << m_dists.at<float>(i,1)<<endl;
            //if(m_dists.at<float>(i,0) <= 3)
        {
            mpts_1.push_back(objectKeypoints.at(i).pt);
            indexes_1.push_back(i); //estos son los ptppairs.
            mpts_2.push_back(imageKeypoints.at(m_indices.at<int>(i,0)).pt);
            indexes_2.push_back(m_indices.at<int>(i,0)); //ptppairs.
        }
    }

    // FIND HOMOGRAPHY
    //cout<<"El tamaño de esta mugre es: "<< mpts_1.size()<<endl ;
    if(mpts_1.size() >= recTreshold)
    {
        objectFounded = true;
        cv::Mat H = findHomography(mpts_1, mpts_2, cv::RANSAC, 1.0, outlier_mask);
        //-- Get the corners
        std::vector<cv::Point2f> obj_corners(4);
        obj_corners[0] = cvPoint(0, 0);
        obj_corners[1] = cvPoint(object->width, 0);
        obj_corners[2] = cvPoint(object->width, object->height);
        obj_corners[3] = cvPoint(0, object->height);
        cv::perspectiveTransform( obj_corners, scene_corners, H);
    } else {
        objectFounded = false;
    }
    return objectFounded;


}

double OrbRecognizer::compareORBDescriptors(const float* d1, const float* d2, double best, int length)
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


void OrbRecognizer::TrainingSet(string dir)
{
    string path, pathN;
    string trainsetP[NumMaxTrainingSet], trainsetN[NumMaxTrainingSet];
    path.append(dir.c_str());
    pathN.append(dir.c_str());
    path.append("pos/");
    pathN.append("neg/");
    DIR *pdir = NULL; // remember, it's good practice to initialise a pointer to NULL!
    pdir = opendir (path.c_str()); // "." will refer to the current directory
    cout<<"nombre de Carpeta es: "<<path.c_str()<<endl;
    dirent *pent = NULL;
    if (pdir == NULL) // mala apertura...
    {
        printf ("\nERROR! carpeta no abre");
        return; // exit the function
    } // end if
    int kp = 0;
    while (pent = readdir (pdir))
    {
        if (pent == NULL) // if pent has not been initialised correctly
        {   // print an error message, and exit the program
            printf ("Debería salirme? yo digo que NO\n");
            return; // exit the function
        }

        if(strcmp(pent->d_name,"..") == 0)
        {
        }
        else
        {
            if(strcmp(pent->d_name,".") == 0)
            {

            }
            else
            {
                if(strcmp(pent->d_name,".directory") != 0)
                {   printf ("Archivo es: %s\n", pent->d_name);
                    trainsetP[kp].append(path.c_str());
                    trainsetP[kp++].append(pent->d_name);

                }
            }
        }

    }
    closedir (pdir);
//     printf("Caso Siguiente: \n");
    int kn=0;
//     pdir = opendir (pathN.c_str()); // "." will refer to the current directory
//     if (pdir == NULL) // mala apertura...
//     {
//         printf ("\nERROR! carpeta no abre");
//         return; // exit the function
//     } // end if
//
//     while (pent = readdir (pdir))
//     {
//         if (pent == NULL) // if pent has not been initialised correctly
//         {   // print an error message, and exit the program
//             printf ("Debería salirme? yo digo que NO\n");
//             return; // exit the function
//         }
//         if(strcmp(pent->d_name,"..") == 0)
//         {
//         } else {
//             if(strcmp(pent->d_name,".") != 0)
//             {
//                 //printf ("Archivo es: %s\n", pent->d_name);
//                 trainsetN[kn].append(pathN.c_str());
//                 trainsetN[kn++].append(pent->d_name);
//             }
//         }
//     }
//     closedir (pdir);

    // Ya tengo guardada la informacion de los nombres de las variables.
    // Para cada imagen calculo sus keypoints y sus descripciones.
    NumTrainSet = kp;
    trainKeypoints = new vector<KeyPoint>*[NumTrainSet];
    trainDescriptors = new Mat*[NumTrainSet];
    for(int j= 0; j< NumTrainSet; j++)
    {
        IplImage* Imgris  = cvLoadImage( trainsetP[j].c_str(), CV_LOAD_IMAGE_GRAYSCALE );
        IplImage* Imcolor = cvLoadImage( trainsetP[j].c_str(), CV_LOAD_IMAGE_COLOR );
        if(!Imgris) {
            cout <<  "Can not load: " <<  trainsetP[j].c_str() << endl ;
            exit(-1);
        }

        int x = Imgris->width; // ancho = eje X = width
        int y = Imgris->height; // altura = eje Y = HEIGHT.
//         printf("Imagen %d: %s tiene Dimensiones (fil: %d, col: %d)\n",j, trainsetP[j].c_str(), y,x);

        Mat objectMat(Imgris);

        // pKeyPoint = &trainKeypoints[kp];
        trainKeypoints[j] = new vector<KeyPoint>;
        trainDescriptors[j] = new Mat;
        switch(TypeMachine)
        {
        case 1:
            OrbMachine->detect(objectMat,  *trainKeypoints[j]);
            OrbMachine->compute(objectMat, *trainKeypoints[j], *trainDescriptors[j]);
            break;
        case 2:
            SurfMachine->detect (objectMat, *trainKeypoints[j]);
            SurfMachine->compute(objectMat, *trainKeypoints[j], *trainDescriptors[j]);
            break;
        case 3:
            SiftMachine->detect (objectMat, *trainKeypoints[j]);
            SiftMachine->compute(objectMat, *trainKeypoints[j], *trainDescriptors[j]);
            break;
        }
        //


    }

    for(int a = 0; a < NumTrainSet; a++)
    {
        for(int b = a; b < NumTrainSet; b++)
        {
            if(a != b) // Se comparan todos contra todos, excepto vs el mismo.
            {
                vector<Point2f> mpts_1, mpts_2;
                vector<int> indexes_1, indexes_2 ;
                bool Valor = FindPairs(*trainKeypoints[a], *trainDescriptors[a], *trainKeypoints[b], *trainDescriptors[b], mpts_1, mpts_2, indexes_1, indexes_2);
//                 printf(" Par: (%d, %d). Resultado: %d. \n",a,b, Valor);
            }

        }
    }


}

void OrbRecognizer::ExecuteTraining()
{
}

bool OrbRecognizer::ExecuteTesting(vector<KeyPoint> recorteKeypoints, Mat recorteDescriptors)
{

    bool Answer;
    Answer = false;
    for(int b = 0; b < NumTrainSet; b++)
    {
        vector<Point2f> mpts_1, mpts_2;
        vector<int> indexes_1, indexes_2 ;
        bool Valor = FindPairs(recorteKeypoints, recorteDescriptors, *trainKeypoints[b], *trainDescriptors[b], mpts_1, mpts_2, indexes_1, indexes_2);
        Answer = Answer || Valor;
        // printf("Comparando con: %d. Resultado: %d.\n",b, Valor);
    }
    return Answer;
}

bool OrbRecognizer::FindPairs( vector<KeyPoint> objectKeypoints, Mat objectDescriptors,
                               vector<KeyPoint> imageKeypoints,  Mat imageDescriptors,
                               vector<Point2f>& mpts_1, vector<Point2f>& mpts_2,
                               vector<int>& indexes_1, vector<int>& indexes_2)
{
    int VALOR;

    if(TypeDistance) {
        VALOR = CV_32FC1;
    } else {
        VALOR = CV_8U;
    }

    Mat m_object(objectDescriptors.rows, objectDescriptors.cols, VALOR); //length es columnas
    Mat m_image (imageDescriptors.rows,  imageDescriptors.cols,  VALOR);
    imageDescriptors.convertTo(m_image, m_image.type());
    objectDescriptors.convertTo(m_object,m_object.type());

    // find nearest neighbors using FLANN
    Mat m_indices(objectDescriptors.rows, 2, CV_32SC1);
    Mat m_dists  (objectDescriptors.rows, 2, CV_32FC1);
    Mat m_indicesINV(imageDescriptors.rows, 2, CV_32SC1);
    Mat m_distsINV  (imageDescriptors.rows, 2, CV_32FC1);


    if (m_object.type() == CV_8U) {
        flann::Index flann_Index   (m_image,  flann::LshIndexParams(12, 20, 2), cvflann::FLANN_DIST_HAMMING);
        flann::Index flann_IndexINV(m_object, flann::LshIndexParams(12, 20, 2), cvflann::FLANN_DIST_HAMMING);

        flann_Index.knnSearch   (m_object, m_indices,    m_dists,    2, cv::flann::SearchParams() );
        flann_IndexINV.knnSearch(m_image,  m_indicesINV, m_distsINV, 2, cv::flann::SearchParams() );
    } else {
        //FLANN_DIST_L2 es preferida para SURF
        flann::Index flann_Index   (m_image,  flann::KDTreeIndexParams(), cvflann::FLANN_DIST_MANHATTAN);
        flann::Index flann_IndexINV(m_object, flann::KDTreeIndexParams(), cvflann::FLANN_DIST_MANHATTAN);

        flann_Index.knnSearch   (m_object, m_indices,    m_dists,    2, cv::flann::SearchParams() );
        flann_IndexINV.knnSearch(m_image,  m_indicesINV, m_distsINV, 2, cv::flann::SearchParams() );
    }

    //cout<<"Distancia: "<<m_dists.row(1)<<endl;
    int C1 = 0;
    int C2 = 0;
    for(int k = 0; k < m_indices.rows; k++)
    {
        int p2 = m_indices.at<int>(k,0);
        int p1 = m_indicesINV.at<int>(p2, 0);
        int d1 = m_dists.at<float>(k,0);
        int d2 = m_dists.at<float>(k,1);
        int dI1 = m_distsINV.at<float>(p2, 0);
        int dI2 = m_distsINV.at<float>(p2, 1);
        if( (k == p1) && ( d1 <= (nndrRatio*d2) ) && ( dI1 <= (nndrRatio * dI2)) )
        {
            C1++;
        }
    }
    for(int j = 0; j < m_indicesINV.rows; j++)
    {
        int p1 = m_indicesINV.at<int>(j,0);
        int p2 = m_indices.at<int>(p1,0);

        if( (j == p2)  )
        {
            C2++;
        }
    }
    //printf("Freq C1 C2: (%d, %d). puntos: (%d, %d) ",C1, C2, m_indices.rows, m_indicesINV.rows);

//     for(unsigned int i = 0; i < m_indices.rows; ++i)
//     {
//       if(m_dists.at<float>(i,0) <= nndrRatio * m_dists.at<float>(i,1))
//       {
// 	      //printf("Las dos distancias que son cercanas son: [%g] y [%g] \n",m_dists.at<float>(i,0), m_dists.at<float>(i,1));
// 	      mpts_1.push_back(objectKeypoints.at(i).pt);
// 	      indexes_1.push_back(i); //estos son los ptppairs.
// 	      mpts_2.push_back(imageKeypoints.at(m_indices.at<int>(i,0)).pt);
// 	      indexes_2.push_back(m_indices.at<int>(i,0)); //ptppairs.
//       }
//     }

    if(C1 >= recTreshold)
    {
        objectFounded = true;
    } else {
        objectFounded = false;
    }

    return objectFounded;


}

bool OrbRecognizer::evalWindow(Mat Ventanita)
{
    vector<KeyPoint> imageKeypoints;
    Mat imageDescriptors;
    switch(TypeMachine)
    {
    case 1:
        OrbMachine->detect(Ventanita, imageKeypoints);
        break;
    case 2:
        SurfMachine->detect(Ventanita, imageKeypoints);
        break;
    case 3:
        SiftMachine->detect(Ventanita, imageKeypoints);
        break;
    default:
        cout<<"No hay máquina aplicable"<<endl;
    }
    if (imageKeypoints.size() < 3)
    {
        cout<< "Pocos puntos."<<endl;
        return false;
    }
    switch(TypeMachine)
    {
    case 1:
        OrbMachine->compute(Ventanita, imageKeypoints, imageDescriptors);
        break;
    case 2:
        SurfMachine->compute(Ventanita, imageKeypoints, imageDescriptors);
        break;
    case 3:
        SiftMachine->compute(Ventanita, imageKeypoints, imageDescriptors);
        break;
    default:
        cout<<"No hay máquina aplicable"<<endl;
    }
    bool objectFound = ExecuteTesting(imageKeypoints, imageDescriptors);
    return objectFound;
}

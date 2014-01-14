/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   plugin1.h
/// \author Martin Reddy
/// \brief  An example plugin built against the plugin API.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///


#include <iostream>
#include <pthread.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <string>

#include "../Gui/gui.h"
#include "../Gui/video.h"
#include "../Plugin_API/pluginapi.h"
#include "../Shared_memory/Robot.h"

#include "../Gui/input_singleton.h"
#include "../Gui/gui.h"
#include "../Gui/tab.h"
#include "../Gui/console.h"
#include "../Gui/video.h"
#include "../Gui/kinect.h"

#include "../MDP/MDP.h"
#include "findobject.h"

Robot *patrol;

video *vid;
console *cons;
tab *current_tab;
Input_Singleton *teclado;

#define WIDTH 1440
#define HEIGHT 900

#define PLUGIN_NAME "Vision_Objects"


template <class T>
string numbertoString(T val) {
    std::ostringstream sstream;
    sstream << val;
    std::string varAsString = sstream.str();
    return varAsString;
}



class Vision_Objects : public IPlugin
{
public:
    void Main();
    void run();
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Vision_Objects;
}

PLUGIN_FUNC void DestroyRenderer(IPlugin *r)
{
    delete r;
}

PLUGIN_DISPLAY_NAME(PLUGIN_NAME);

PLUGIN_INIT()
{
    // register our new renderer

    RegisterPlugin(PLUGIN_NAME, CreatePlugin, DestroyRenderer);
    return 0;
}

std::string accion;

void Vision_Objects::Main()
{
    int polo=0;

    GUI::GetInstance();

    IplImage *frame;
    current_tab = new tab("Vision_Objects");
    vid = new video(0,HEIGHT*.02,WIDTH/2,HEIGHT/2,"Vision_Objects", current_tab);

//     do
//     {
//        accion=patrol->get_Instance().get_Action();
//
//     }while (accion!="confirmar_objeto");

    FindObject* fo=new FindObject();
    ToolsImages tis;


    //PARA DIBUJAR UNA IMAGEN
    static CvScalar colors[] =
    {   {{0,0,255}},//azul
        {{0,128,255}},//
        {{0,255,255}},//
        {{0,255,0}},//verde
        {{255,128,0}},
        {{255,255,0}},
        {{255,0,0}},//rojo
        {{255,0,255}},
        {{255,255,255}}//blanco
    };

    /*****se eliminará en version final********/
//PARAMETROS PARA LA LECTURA DE UN VIDEO
    int numero_de_muestras=1000000;
    int velocidad_muestreo=2;
    int contador_muestras=0;
    int contador_velocidad=0;
    int count_finded=0;
    int count_scenesProcessed=0;
    bool help=false;
    /*******************************************/

    //PARAMETROS DEL ALGORITMO
    int n_best=8;
    int numTemplates=2;

    int h=6;
    int s=4;
    int v=4;
    int samplesx=15;
    int samplesy=15;
    int numHistox=2;
    int numHistoy=3;
    int numEscalas=3;
    double umbralCorrelation=0.4;


    //1=correlation, 2=differences 3 combination
    bool  spacecolorc1c2c3=false;
    int medidaSimilitud=3;
    int numeroCanalesColor=3;

    //ARCHIVOS DE ENTRADA
    /*****se eliminará en version final********/
//                    string filevideo="Videos/video_1.avi";
    /*******************************************/
    string* fileTemplate=new string[numTemplates];
    string* fileTemplate_msk=new string[numTemplates];

    fileTemplate[0]="../data/Imagen/P4_1.png";
    fileTemplate[1]="../data/Imagen/P4_2.png";
    //   fileTemplate[2]="Imagen/P4_3.png";

    fileTemplate_msk[0]="../data/Imagen_msk/P4_1msk.png";
    fileTemplate_msk[1]="../data/Imagen_msk/P4_2msk.png";
    // fileTemplate_msk[2]="Imagen_msk/P4_3msk.png";


    cvNamedWindow( "Finding Objetc", CV_WINDOW_AUTOSIZE );

    //INDICAR SI TOMAMOS CAPTURA DE VIDEO O VIDEO PRETOMADO
    /***** se eliminará en version final********/
    //    CvCapture* capture = cvCreateFileCapture( filevideo.c_str());
    /*******************************************/
    // CvCapture* capture =cvCaptureFromCAM( -1 );


    //INICIALIZANDO IMAGENES


    IplImage *scene_c1c2c3 = 0;
    IplImage *scene = 0;
    int n_b=n_best;
    IplImage** object_c1c2c3= new IplImage*[numTemplates];
    IplImage** object_rgb =new IplImage*[numTemplates];

    IplImage** object_rgb_msk=new IplImage*[numTemplates];

    cv::Mat** Histograms_template=new cv::Mat*[numTemplates];

    //surf points
    CvSeq** objectKeyPoint=new CvSeq*[numTemplates];
    CvSeq** objectDescriptors=new CvSeq*[numTemplates];

    //CARGAR LOS TEMPLATES

    for(int k=0; k<numTemplates; k++)
    {

        object_rgb[k]=(cvLoadImage( (fileTemplate[k]).c_str(), CV_LOAD_IMAGE_COLOR));
        object_rgb_msk[k] = cvLoadImage( (fileTemplate_msk[k]).c_str(), CV_LOAD_IMAGE_GRAYSCALE);
        if(spacecolorc1c2c3) {
            object_c1c2c3[k] = tis.getc1c2c3(object_rgb[k]);
            (new Histograma )->CalcularHistogramas3Canal(object_c1c2c3[k],object_rgb_msk[k],Histograms_template[k],numHistox,numHistoy,h,s,v);

            if(numeroCanalesColor==1) {

            }

        }
        else {
            object_c1c2c3[k]=cvCreateImage(cvSize(object_rgb[k]->width, object_rgb[k]->height),8,3);
            cvCvtColor( object_rgb[k],object_c1c2c3[k], CV_RGB2HSV);
            Histograma().CalcularHistogramasHSV(object_c1c2c3[k],object_rgb_msk[k],Histograms_template[k],numHistox,numHistoy,h,s,v);
            if(numeroCanalesColor==1) {
            }
        }

        //calcular puntos SURF de templates
        tis.getKeyPoints(object_rgb[k],object_rgb_msk[k],&objectKeyPoint[k],&objectDescriptors[k]);
    }


    //PROCESAR EL VIDEO

    //INSTRUCCIONES PARA GUARDAR EL VIDEO
    /***** se eliminará en version final********/
    /*   IplImage* image2 = cvQueryFrame(capture);
       double fps = cvGetCaptureProperty(
               capture, CV_CAP_PROP_FPS);
       CvSize imgSize;
       imgSize.width = image2->width;
       imgSize.height = image2->height;


       CvVideoWriter *writer = cvCreateVideoWriter(outvideo.toStdString().c_str(),
               CV_FOURCC('M', 'J', 'P', 'G'),
               fps,imgSize);*/

    /*******************************************/


    while(contador_muestras++<numero_de_muestras) {
        scene = patrol->get_Instance().Ptz.get_image();
        cvFlip(scene, scene, 1);
        int n_temp=0;
        bool objectFinded=false;
        ImageLocation* thebest;
        CvSeq* sceneKeyPoint;
        CvSeq* sceneDescriptors;

        if( !scene ) break;
        contador_velocidad++;
        if(contador_velocidad>velocidad_muestreo) {
            help=true;
            contador_velocidad=0;
        }
        if(help) {
            n_best=n_b;
            tis.getKeyPoints(scene,&sceneKeyPoint,&sceneDescriptors);
            if(spacecolorc1c2c3) {
                scene_c1c2c3 = tis.getc1c2c3(scene);
                if(numeroCanalesColor==1) {
                    //    scene_c1c2c3_canal1=Histograma().CalculeIMGlabels(scene_c1c2c3,h,s,v);
                }
            }

            else {
                scene_c1c2c3=cvCreateImage(cvSize(scene->width, scene->height),8,3);
                cvCvtColor( scene,scene_c1c2c3, CV_RGB2HSV);
                if(numeroCanalesColor==1) {
                    //  scene_c1c2c3_canal1=Histograma().CalculeIMGlabelsHSV(scene_c1c2c3,h,s,v);
                }
            }

            count_scenesProcessed++;

            ImageLocation** placesSceneCorrelation;
            ImageLocation** placesSceneHistograms;
            ImageLocation** placesSceneSURF;

            priority_queue<ImageLocation*,vector<ImageLocation*>,ComparePoint> P1s;


            double tt = (double)cvGetTickCount();

            /**PRIMER FILTRADO**/
            for(int k=0; k<numTemplates; k++) {
                fo->FindByHistograms(scene_c1c2c3,Histograms_template[k],object_rgb_msk[k],placesSceneHistograms,n_best,samplesx,samplesy,numEscalas, numHistox, numHistoy,h,s,v,spacecolorc1c2c3);
                for(int l=0; l<n_best; l++)
                    placesSceneHistograms[l]->templateNumber=k;

                ImageLocation* salidasurf;
                bool hay= fo->FindBySURF_define(sceneKeyPoint,sceneDescriptors,objectKeyPoint[k],objectDescriptors[k],placesSceneHistograms,n_best,salidasurf);
                /**agregar los mejores a la priority queue**/
                if(hay)
                    P1s.push(salidasurf);
            }//fin de templates
            tt = (double)cvGetTickCount() - tt;

            printf( " time processing frame= %gs\n", tt/(cvGetTickFrequency()*1000000));


            /*****ENCONTRO OBJETO*****/
            if(!P1s.empty())
            {

                thebest=P1s.top();

                CvPoint pCorrelation1;
                CvPoint pCorrelation2;
                pCorrelation1.x=thebest->x;
                pCorrelation1.y=thebest->y;
                pCorrelation2.x=thebest->x+thebest->width;
                pCorrelation2.y=thebest->y+thebest->height;

                if(thebest->value>=umbralCorrelation) {
                    n_temp=thebest->templateNumber;
                    fo->drawRect(scene,pCorrelation1,pCorrelation2,colors[7]);

                    count_finded++;
                    objectFinded=true;

                    printf( " FINDED= %g \n", thebest->value);

                    P1s.pop();
                }

            }
            while(!P1s.empty())
                P1s.pop();

            help=false;
            cvReleaseImage(&scene_c1c2c3);

        }
        IplImage* correspond = cvCreateImage( cvSize(scene->width, (object_rgb[n_temp])->height+scene->height), 8, 3 );
        cvSetImageROI( correspond, cvRect( 0, 0, object_rgb[n_temp]->width, object_rgb[n_temp]->height ) );
        cvCopy( object_rgb[n_temp], correspond );
        cvSetImageROI( correspond, cvRect( 0, object_rgb[n_temp]->height, correspond->width, correspond->height ) );
        cvCopy( scene, correspond );
        cvResetImageROI( correspond );
        /*****se eliminará en version final********/
        //pintar correspondencis si las hay
        if(objectFinded) {
            CvRect ubicationScene;
            ubicationScene.x=thebest->x;
            ubicationScene.y=thebest->y;
            ubicationScene.width=thebest->width;
            ubicationScene.height=thebest->height;
            vector<int> indicesScene;
            vector<int> indicesObjectMatch;
            vector<int> indicesSceneMatch;

            MatchingSURF().pointsInRect(sceneKeyPoint,sceneDescriptors,indicesScene,ubicationScene);
            MatchingSURF().findMatchingKeyPoints2_1(objectKeyPoint[n_temp],objectDescriptors[n_temp],sceneKeyPoint,sceneDescriptors,indicesScene,indicesObjectMatch,indicesSceneMatch);

            int nmatch=indicesObjectMatch.size();
            for(int i=0; i<nmatch; i++) {
                const CvSURFPoint* KeyPointObject = (const CvSURFPoint*) cvGetSeqElem(objectKeyPoint[n_temp], indicesObjectMatch.at(i));
                const CvSURFPoint* KeyPointScene = (const CvSURFPoint*) cvGetSeqElem(sceneKeyPoint, indicesSceneMatch.at(i));

                CvPoint psurf_obj;
                CvPoint psurf_scn;
                psurf_obj.x=KeyPointObject->pt.x;
                psurf_obj.y=KeyPointObject->pt.y;
                psurf_scn.x=KeyPointScene->pt.x;
                psurf_scn.y=object_rgb[n_temp]->height+KeyPointScene->pt.y;
                cvDrawLine(correspond,psurf_obj,psurf_scn,colors[2]);
            }

            patrol->get_Instance().Sintetizer.set_Phrase("i found coca");
            vid->SetImage(correspond);
            sleep(3);
            patrol->get_Instance().set_Action(cambiar_estado("objeto_confirmado", "si"));
// 			      contador_muestras=numero_de_muestras+1;
        }
        /*******************************************/

        vid->SetImage(correspond);
        // cvShowImage( "Finding Objetc", correspond );
        //cvWriteFrame(writer, correspond);
        char c = cvWaitKey(33);
        if( c == 27 ) break;

        cvReleaseImage(&correspond);





    }//fin de while

    for(;;);
    //cvReleaseCapture( &capture );
    //cvReleaseVideoWriter(&writer);



}



void Vision_Objects::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}


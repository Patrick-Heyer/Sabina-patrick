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

#include "../MDP/MDP.h"
#include "findobject.h"

Robot *patrol;

Video *videoDisplay;
Console *cons;
Tab *pluginTab;
InputSingleton *teclado;

template <class T>
string numbertoString(T val){
    std::ostringstream sstream;
    sstream << val;
    std::string varAsString = sstream.str();
    return varAsString;
}

vector<int> deletefromlist(vector<int> vnf, vector<int> vf){
	vector<int> newvector;
	
	for(int i=0;i<vnf.size();i++){
		bool isj=false;
		for(int j=0;j<vf.size();j++)
			if(vnf.at(i)==vf.at(j)){
				isj=true;
				break;
			}
		if(!isj)
			newvector.push_back(vnf.at(i));
	}
	return newvector;
}

int indexOfList(int* vector,int total,  int value){
    int index=0;
    for(int i=0;i<total;i++)
        if(vector[i]==value)
			return i;
	
	
    return index;
	
}

#define PLUGIN_NAME "Vision_Objects"


/**
 * @brief Vision_Objects plugin
 * 
 * This plugin is for object recognition.
 * @author     Patrick Heyer, patrickhey@prodigy.net.mx
 * @date       jul 04, 2012
 * @version    1.0
 **/
class Vision_Objects : public IPlugin
{
public:
    void Main();
	void stop();
    void run();
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Vision_Objects;
}

PLUGIN_FUNC void DestroyPlugin(IPlugin *r)
{
    delete r;
}

PLUGIN_DISPLAY_NAME(PLUGIN_NAME);

PLUGIN_INIT()
{
    // register our new plugin
    std::cout << "PLUGIN_INIT" << std::endl;
    RegisterPlugin(PLUGIN_NAME, CreatePlugin, DestroyPlugin);
    return 0;
}

std::string accion;

void Vision_Objects::Main()
{
	
	/**
	 * \sa cambiar_estado
	 **/
    int polo=0;

    Gui::getInstance();
    
    IplImage *frame;
    pluginTab = new Tab("Vision_Objects");
    videoDisplay = new Video(0,HEIGHT*.02,WIDTH/2,HEIGHT/2,"Vision_Objects", pluginTab);
    
    do
    {
       accion=patrol->getInstance().get_Action();
      
    }while (accion!="take_frame");
    
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
    int numero_de_muestras=1000;
    int velocidad_muestreo=10;
    int contador_muestras=0;
    int contador_velocidad=0;
    int count_found=0;
    int count_scenesProcessed=0;
    bool help=false;
	// ARCHIVOS DONDE SE GUARDARAN LOS RESULTADOS
	string dirResultados="../data/results/";
	string file=dirResultados+"estadisticas.txt";
	string outvideo=dirResultados+"video_result.avi";
	/*  QFile salida(file.c_str());
	 salida.open(QIODevice::WriteOnly | QIODevice::Text);
	 QTextStream out(&salida);*/
	/*******************************************/
	
	//PARAMETROS DEL ALGORITMO
	int n_best=8;
	int numTemplates=1;
	int num_productos=16;
	
	int h=6;
	int s=4;
	int v=4;
	int samplesx=10;
	int samplesy=10;
	int numHistox=2;
	int numHistoy=3;
	int numEscalas=3;
	double umbralCorrelation=0.4;
	//1=correlation, 2=differences 3 combination
	bool  spacecolorc1c2c3=false;
	int medidaSimilitud=3;
	int numeroCanalesColor=3;
	
	int auxiliar_i=0;
	// vector that save products not found yet.
	vector<int> productsNotfound;
	vector<int> productsfound;
	
	
	//ARCHIVOS DE ENTRADA
	/*****se eliminará en version final********/
// 	string filevideo="VideosNEW/webcam1.avi";
	/*******************************************/
	int* prNumber=new int[num_productos];
	string** fileTemplate=new string*[num_productos];
	string** fileTemplate_msk=new string*[num_productos];
	string*  products_names=new string[26];
	
	for(int i=0;i<num_productos;i++){
		fileTemplate[i]=new string[numTemplates];
		fileTemplate_msk[i]=new string[numTemplates];
	}
	products_names[0]="coke";           products_names[1]="seven_up";          products_names[2]="lemon_tea";
	products_names[3]="pepsi";           products_names[4]="mineral_water";          products_names[5]="gatorade";
	products_names[6]="orange_juice";           products_names[7]="apple_juice";          products_names[8]="crackers";
	products_names[9]="wafer";           products_names[10]="candy";          products_names[11]="cookies";
	products_names[12]="chili";           products_names[13]="dough";          products_names[14]="tomatoe_sauce";
	products_names[15]="soy_sauce";           products_names[16]="tofu";          products_names[17]="tea_box";
	products_names[18]="cotton";           products_names[19]="desinfectant";          products_names[20]="hair_gel";
	products_names[21]="soap";           products_names[22]="tooth_paste";          products_names[23]="toilet_paper";
	products_names[24]="shampoo";           products_names[25]="pad";
	
	
	
	/*please coment no used*/
	prNumber[0]=1;    prNumber[1]=2;  prNumber[2]=3;      prNumber[3]=4;
	prNumber[4]=6;    prNumber[5]=7;  prNumber[6]=8;      prNumber[7]=13;
	prNumber[8]=14;    prNumber[9]=15;  prNumber[10]=16;      prNumber[11]=21;
	prNumber[12]=22;  prNumber[13]=23;  prNumber[14]=25;      prNumber[15]=26;
	/*  prNumber[16]=17;    prNumber[17]=18;  prNumber[18]=19;      prNumber[19]=20;
	 prNumber[20]=21;    prNumber[21]=22;  prNumber[22]=23;      prNumber[23]=24;
	 prNumber[24]=25;    prNumber[25]=26;*/
	//charging objects for find
	for(int i=0;i<num_productos;i++)
		productsNotfound.push_back(prNumber[i]);
	
	//loading filenames
	for(int i=0;i<num_productos;i++)
		for(int j=0;j<numTemplates;j++){
			string patron="../data/Imagen/p"+numbertoString(prNumber[i])+"_"+numbertoString(j+1)+".png";
			fileTemplate[i][j]=patron;
			string patronmsk="../data/Imagen_msk/p"+numbertoString(prNumber[i])+"_"+numbertoString(j+1)+"msk.png";
			fileTemplate_msk[i][j]=patronmsk;
		}
	
	
	
	//INDICAR SI TOMAMOS CAPTURA DE VIDEO O VIDEO PRETOMADO
	/***** se eliminará en version final********/
	//CvCapture* capture = cvCreateFileCapture( filevideo.c_str());
	/*******************************************/
	//  CvCapture* capture =cvCaptureFromCAM( -1 );
	
	
	//INICIALIZANDO IMAGENES
	IplImage *scene_c1c2c3 = 0;
	IplImage *scene = 0;
	int n_b=n_best;
	IplImage*** object_c1c2c3= new IplImage**[num_productos];
	IplImage*** object_rgb =new IplImage**[num_productos];
	IplImage*** object_rgb_msk=new IplImage**[num_productos];
	
	cv::Mat*** Histograms_template=new cv::Mat**[num_productos];
	
	//surf points
	CvSeq*** objectKeyPoint=new CvSeq**[num_productos];
	CvSeq*** objectDescriptors=new CvSeq**[num_productos];
	
	for(int j=0;j<num_productos;j++){
		object_c1c2c3[j]=new IplImage*[numTemplates];
		object_rgb[j]=new IplImage*[numTemplates];
		object_rgb_msk[j]=new IplImage*[numTemplates];
		Histograms_template[j]=new cv::Mat*[numTemplates];
		objectKeyPoint[j]=new CvSeq*[numTemplates];
		objectDescriptors[j]=new CvSeq*[numTemplates];
	}
	
	//CHARGING LOS TEMPLATES of DATA BASE
	printf( "starting charge database..  \n");
	
	for(int i=0;i<num_productos;i++)
		for(int k=0;k<numTemplates;k++)
		{
			object_rgb[i][k]=(cvLoadImage( (fileTemplate[i][k]).c_str(), CV_LOAD_IMAGE_COLOR));
			object_rgb_msk[i][k] = cvLoadImage( (fileTemplate_msk[i][k]).c_str(), CV_LOAD_IMAGE_GRAYSCALE);
			if(spacecolorc1c2c3){
				object_c1c2c3[i][k] = tis.getc1c2c3(object_rgb[i][k]);
				(new Histograma )->CalcularHistogramas3Canal(object_c1c2c3[i][k],object_rgb_msk[i][k],Histograms_template[i][k],numHistox,numHistoy,h,s,v);
				
				if(numeroCanalesColor==1){
					
				}
				
			}
			else{
				object_c1c2c3[i][k]=cvCreateImage(cvSize(object_rgb[i][k]->width, object_rgb[i][k]->height),8,3);
				cvCvtColor( object_rgb[i][k],object_c1c2c3[i][k], CV_RGB2HSV);
				Histograma().CalcularHistogramasHSV(object_c1c2c3[i][k],object_rgb_msk[i][k],Histograms_template[i][k],numHistox,numHistoy,h,s,v);
				if(numeroCanalesColor==1){
				}
			}
			
			//calcular puntos SURF de templates
			tis.getKeyPoints(object_rgb[i][k],object_rgb_msk[i][k],&objectKeyPoint[i][k],&objectDescriptors[i][k]);
		}
	
	
	
	//INSTRUCCIONS for save VIDEO
	/***** se eliminará en version final********/
	//IplImage* image2 = patrol->getInstance().Ptz.get_image();
// 	double fps = cvGetCaptureProperty(  capture, CV_CAP_PROP_FPS);
// 	CvSize imgSize;
// 	imgSize.width = image2->width;
// 	imgSize.height = image2->height;
	
	
	//CvVideoWriter *writer = cvCreateVideoWriter(outvideo.c_str(),
	//											CV_FOURCC('M', 'J', 'P', 'G'),
	//											14.985,imgSize);
	
	/*******************************************/
	
	
	
	
	printf( "STARTING SEARCHER  \n");
	int movimientos=0;
	for(;;)
	{
	  
	  if(patrol->getInstance().get_Action()=="take_frame")
	  {
	    IplImage* img_salida;
	    cvNamedWindow( "Finding Objetc", CV_WINDOW_AUTOSIZE );
		scene = patrol->getInstance().Ptz.get_image();
		
		int n_temp=0;
		ImageLocation* thebest;
		CvSeq* sceneKeyPoint;
		CvSeq* sceneDescriptors;
		
		
			
			
			tis.getKeyPoints(scene,&sceneKeyPoint,&sceneDescriptors);
			if(spacecolorc1c2c3){
				scene_c1c2c3 = tis.getc1c2c3(scene);
				if(numeroCanalesColor==1){
					
				}
			}
			
			else{
				scene_c1c2c3=cvCreateImage(cvSize(scene->width, scene->height),8,3);
				cvCvtColor( scene,scene_c1c2c3, CV_RGB2HSV);
				if(numeroCanalesColor==1){
					
				}
			}
			
			count_scenesProcessed++;
			productsfound.clear();
			
			/*FIRST FILTER*/
			printf( "NEW FRAME SEARCHING %d  PRODUCTS \n",productsNotfound.size());
			
			
			double tt = (double)cvGetTickCount();
			for(int countProductsSearching=0;countProductsSearching<productsNotfound.size();countProductsSearching++){
				bool objectfound=false;
				
				
				n_best=n_b;
				auxiliar_i=productsNotfound.at(countProductsSearching)-1;
				int index_aux=indexOfList(prNumber,num_productos,auxiliar_i+1);
				//printf( "Searching producto: %d  \n",auxiliar_i+1 );
				
				ImageLocation** placesSceneHistograms;
				priority_queue<ImageLocation*,vector<ImageLocation*>,ComparePoint> P1s;
				ImageLocation* salidasurf;
				
				for(int k=0;k<numTemplates;k++){
					fo->FindByHistograms(scene_c1c2c3,Histograms_template[index_aux][k],object_rgb_msk[index_aux][k],placesSceneHistograms,n_best,samplesx,samplesy,numEscalas, numHistox, numHistoy,h,s,v,spacecolorc1c2c3);
					for(int l=0;l<n_best;l++)
						placesSceneHistograms[l]->templateNumber=k;
					
					bool hay= fo->FindBySURF_define(sceneKeyPoint,sceneDescriptors,objectKeyPoint[index_aux][k],objectDescriptors[index_aux][k],placesSceneHistograms,n_best,salidasurf);
					//CAMBIO 2 COMPARAR CORRELACION
					if(hay){
						CvPoint pbest1;
						pbest1.x=salidasurf->x;
						pbest1.y=salidasurf->y;
						
						IplImage* templateScaled=tis.EscalarImagen(object_c1c2c3[index_aux][k],salidasurf->escala*object_c1c2c3[index_aux][k]->width,salidasurf->escala*object_c1c2c3[index_aux][k]->height);
						IplImage* templateScaled_msk=tis.EscalarImagen(object_rgb_msk[index_aux][k],salidasurf->escala*object_c1c2c3[index_aux][k]->width,salidasurf->escala*object_c1c2c3[index_aux][k]->height);
						IplImage* subScenec1c2c3=tis.getSubImage(scene,pbest1,salidasurf->width,salidasurf->height);
						// qDebug()<<templateScaled->width<<"    == "<<subScenec1c2c3->width<<"     "<<templateScaled->height<<"=="<<subScenec1c2c3->height;
						double newFilterCorrelation=ImageSimilarity().Porcent(templateScaled,subScenec1c2c3,templateScaled_msk);
						if(newFilterCorrelation<0.4)
							hay=false;
						cvReleaseImage(&templateScaled);
						cvReleaseImage(&templateScaled_msk);
						cvReleaseImage(&subScenec1c2c3);
						
						// qDebug()<<newFilterCorrelation;
					}
					
					
					
					if(hay)
						P1s.push(salidasurf);
				}//fin de templates
				
				
				
				IplImage* scene2=cvCloneImage(scene);
				
				/***********************OBJECT found*********************/
				
				if(!P1s.empty())
				{
					thebest=P1s.top();
					CvPoint pbest1;
					CvPoint pbest2;
					pbest1.x=thebest->x;
					pbest1.y=thebest->y;
					pbest2.x=thebest->x+thebest->width;
					pbest2.y=thebest->y+thebest->height;
					
					if(thebest->value>=umbralCorrelation){
						n_temp=thebest->templateNumber;
						fo->drawRect(scene2,pbest1,pbest2,colors[7]);
						
						count_found++;
						objectfound=true;
						
						printf( " %d --- OBJECT FOUND %d  %s whit value= %g \n",count_found, auxiliar_i+1,products_names[auxiliar_i].c_str(),thebest->value);
						patrol->getInstance().Sintetizer.set_Phrase(products_names[auxiliar_i].c_str());
						//new vector of products;
						productsfound.push_back(auxiliar_i+1);
						
						/*****se eliminará en version final********/
						// out<<count_found<<"  OBJECT FOUND"<<auxiliar_i+1<<"   "<<products_names[auxiliar_i].c_str()<<" whit  template="<<thebest->templateNumber<<"     value="<<thebest->value<<"\n";
						
						IplImage* img_found=tis.getSubImage(scene,pbest1,thebest->width,thebest->height);
						img_salida=tis.join2Imagesmsk1(object_rgb[index_aux][thebest->templateNumber],object_rgb_msk[index_aux][thebest->templateNumber],img_found);
						string simagenSalida=dirResultados+"Found"+numbertoString(count_found)+"_product"+numbertoString(auxiliar_i+1)+"_"+products_names[auxiliar_i]+".jpg";
						cvSaveImage(simagenSalida.c_str(),img_salida);
						cvReleaseImage(&img_found);
						/*******************************************/
						
						P1s.pop();
					}
					
				}
				IplImage* correspond = cvCreateImage( cvSize(scene->width, (object_rgb[index_aux][n_temp])->height+scene->height), 8, 3 );
				cvSetImageROI( correspond, cvRect( 0, 0, object_rgb[index_aux][n_temp]->width, object_rgb[index_aux][n_temp]->height ) );
				cvCopy( object_rgb[index_aux][n_temp], correspond );
				cvSetImageROI( correspond, cvRect( 0, object_rgb[index_aux][n_temp]->height, correspond->width, correspond->height ) );
				cvCopy( scene2, correspond );
				cvResetImageROI( correspond );
				
				if(objectfound){
					
					
					CvRect ubicationScene;
					ubicationScene.x=thebest->x;
					ubicationScene.y=thebest->y;
					ubicationScene.width=thebest->width;
					ubicationScene.height=thebest->height;
					vector<int> indicesScene;
					vector<int> indicesObjectMatch;
					vector<int> indicesSceneMatch;
					
					MatchingSURF().pointsInRect(sceneKeyPoint,sceneDescriptors,indicesScene,ubicationScene);
					//MatchingSURF().findMatchingKeyPoints2_1(objectKeyPoint[n_temp],objectDescriptors[n_temp],sceneKeyPoint,sceneDescriptors,indicesScene,indicesObjectMatch,indicesSceneMatch);
					MatchingSURF().findMatchingKeyPoints_1(objectKeyPoint[index_aux][n_temp],objectDescriptors[index_aux][n_temp],sceneKeyPoint,sceneDescriptors,indicesScene,indicesObjectMatch,indicesSceneMatch);
					
					/***pintar correspondencis si las hay**/
					int nmatch=indicesObjectMatch.size();
					for(int i=0;i<nmatch;i++){
						const CvSURFPoint* KeyPointObject = (const CvSURFPoint*) cvGetSeqElem(objectKeyPoint[index_aux][n_temp], indicesObjectMatch.at(i));
						const CvSURFPoint* KeyPointScene = (const CvSURFPoint*) cvGetSeqElem(sceneKeyPoint, indicesSceneMatch.at(i));
						
						CvPoint psurf_obj;
						CvPoint psurf_scn;
						psurf_obj.x=KeyPointObject->pt.x;
						psurf_obj.y=KeyPointObject->pt.y;
						psurf_scn.x=KeyPointScene->pt.x;
						psurf_scn.y=object_rgb[index_aux][n_temp]->height+KeyPointScene->pt.y;
						cvDrawLine(correspond,psurf_obj,psurf_scn,colors[2]);
					}
					
				}//END FOUND WRITE
				/*********************************END FOUND***********************************/
				
				while(!P1s.empty())
					P1s.pop();
				
				help=false;
				
				
				
				productsNotfound=deletefromlist(productsNotfound,productsfound);
				
				cvShowImage( "Finding Objetc", correspond );
// 				cvWriteFrame(writer, correspond);
				cvReleaseImage(&correspond);
				
				char c = cvWaitKey(33);
				if( c == 27 ) break;
				
				if(objectfound){
				  	cvShowImage( "FOUND OBJECT", img_salida );
// 						cvWaitKey(5000);
// 						cvDestroyWindow("FOUND OBJECT");
						
					
//  						cvReleaseImage(&img_salida);

				  
				  
				}
// 				cvReleaseImage(&correspond);
			}//END DIFERENtS PRODUCTS
			cvReleaseImage(&scene_c1c2c3);
			tt = (double)cvGetTickCount() - tt;
			//
			printf( " time processing frame= %gs\n", tt/(cvGetTickFrequency()*1000000));
			
			
	
		
		cvShowImage( "Finding Objetc", scene );
// 		cvWriteFrame(writer, scene);
		//       cvReleaseImage(&scene);
		patrol->getInstance().set_Action("siguiente_punto");
	 cvDestroyWindow("Finding Objetc");
	 movimientos++;
		
	  } //fin de if	
	if (movimientos==6)
	{
// 	  cvReleaseVideoWriter(&writer);
	}
	}//fin de for infinito
	

	
   
}

void Vision_Objects::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}

void Vision_Objects::stop()
{

}

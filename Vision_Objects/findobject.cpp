#include "findobject.h"



void FindObject::drawRect(IplImage *img, CvPoint pini, CvPoint pfin, CvScalar color){

    cvRectangle(img,pini,pfin,color,5);
}

IplImage* FindObject::subImage(IplImage *image, CvRect roi){

    IplImage *result;
    // set ROI, you may use following two funs:
    cvSetImageROI(image,roi);
    // sub-image
    result = cvCreateImage( cvSize(roi.width, roi.height), image->depth, image->nChannels );
    cvCopy(image,result);
    cvResetImageROI(image); // release image ROI
    return result;

}






///////////////////////////////////////////


bool FindObject::FindBySpacial(IplImage *scene, IplImage *templ, IplImage *msk_temp, ImageLocation **&pointOut, int numOut, int samples_width, int samples_Height, int numScalas, bool spaceColorC1c2c3, int medidaSimilitud, int numCanalColor, int numCuantificacion) {
    double inc_Escala=1/double(numScalas);
    priority_queue<ImageLocation*,vector<ImageLocation*>,ComparePoint> spacialOrdered;
    ImageSimilarity isimi;
    int decrementox=double(samples_width-3)/double(numScalas);
    int decrementoy=double(samples_Height-3)/double(numScalas);

    for(int k=0;k<numScalas;k++){
        //Cambiar el tamaño del template
        double escalaActual=(k+1)*inc_Escala;
            int templ_widthNew=escalaActual*templ->width;
            int templ_heightNew=escalaActual*templ->height;

        int iwidth = scene->width - templ_widthNew + 1;
        int iheight = scene->height - templ_heightNew + 1;
        int numPix_x=iwidth/samples_width;
        int numPix_y=iheight/samples_Height;

      CvRect rect;
        rect.height=templ_heightNew;
        rect.width=templ_widthNew;
        ToolsImages ti;
        IplImage* templ_scaled=ti.EscalarImagen(templ,rect.width,rect.height);
        IplImage* templ_msk_scaled=ti.EscalarImagen(msk_temp,rect.width,rect.height);


            int w=double(iwidth)/double(numPix_x);
            int h=double(iheight)/double(numPix_y);

            //INICIA
            for(int i=0;i<h;i++)
              for(int j=0;j<w;j++){
                    rect.x=j*numPix_x;
                    rect.y=i*numPix_y;
                    IplImage* sub=subImage(scene,rect);

                    double valorcito=0;
                    ////////chossing metod, space color and canal
                    //1=correlation, 2=differences 3 combination
                    if(numCanalColor==3){
                            switch(medidaSimilitud){
                                case 1:
                                    valorcito=isimi.correlationMSK3Canal(sub,templ_scaled,templ_msk_scaled,spaceColorC1c2c3);
                                    break;
                                case 2:
                                   valorcito=isimi.DiferenciasCuadrado3Canal(sub,templ_scaled,templ_msk_scaled,spaceColorC1c2c3);
                                   break;
                               default:
                                 valorcito=isimi.SpacialMSK3Canal(sub,templ_scaled,templ_msk_scaled, 0.3,spaceColorC1c2c3);
                                 break;
                            }}
                          else{
                            switch(medidaSimilitud){
                                case 1:
                                    valorcito=isimi.correlationMSK1Canal(sub,templ_scaled,templ_msk_scaled,numCuantificacion,spaceColorC1c2c3);
                                    break;
                                case 2:
                                   valorcito=isimi.DiferenciasCuadrado1Canal(sub,templ_scaled,templ_msk_scaled,numCuantificacion,spaceColorC1c2c3);
                                   break;
                               default:
                                 valorcito=isimi.spacialMSK1Canal(sub,templ_scaled,templ_msk_scaled,numCuantificacion, 0.3,spaceColorC1c2c3);
                                 break;
                      }}
                     ImageLocation* p1=new ImageLocation(rect.x,rect.y,rect.width,rect.height,0,valorcito,escalaActual);
                        spacialOrdered.push(p1);
                    cvReleaseImage(& sub);


                   }//fin ij

        samples_width-=decrementox;
        samples_Height-=decrementoy;

        cvReleaseImage(& templ_scaled);
        cvReleaseImage(& templ_msk_scaled);
}

    pointOut=new ImageLocation*[numOut];
    for(int i=0;i<numOut;i++){
        ImageLocation* minLoc_=spacialOrdered.top();
        pointOut[i]=minLoc_;
        spacialOrdered.pop();
    }
    //liberar Memoria de priority queue
    while(!spacialOrdered.empty())
        spacialOrdered.pop();
    return true;
}




////histogramas
bool FindObject::FindByHistograms(IplImage *scene, cv::Mat *&histo_template, IplImage *msk_temp, ImageLocation **&pointOut, int& numOut, int samples_width, int samples_Height, int numScalas, int numHistox, int numHistoy, int nh, int ns, int nv,bool spaceColorC1c2c3) {
    double inc_Escala=1/double(2*(numScalas-1));
    priority_queue<ImageLocation*,vector<ImageLocation*>,ComparePoint> histogramOrdered;
    ImageSimilarity isimi;
//     int decrementox=double(samples_width-3)/double(numScalas);
//     int decrementoy=double(samples_Height-3)/double(numScalas);
    cv::Mat* histo_scene_template;

    int decrementox=6;
    int decrementoy=6;

    for(int k=0;k<numScalas;k++){
        //Cambiar el tamaño del template
        //CAMBIO 5 metodologías escalas (0.5 en adelante)
        double escalaActual=(2*(numScalas-1)-(k))*inc_Escala;
       // qDebug()<<escalaActual;
            int templ_widthNew=escalaActual*msk_temp->width;
            int templ_heightNew=escalaActual*msk_temp->height;

        int iwidth = scene->width - templ_widthNew + 1;
        int iheight = scene->height - templ_heightNew + 1;
        int numPix_x=iwidth/samples_width;
        int numPix_y=iheight/samples_Height;

      CvRect rect;
        rect.height=templ_heightNew;
        rect.width=templ_widthNew;
        IplImage* templ_msk_scaled=ToolsImages().EscalarImagen(msk_temp,rect.width,rect.height);


            int w=double(iwidth)/double(numPix_x);
            int h=double(iheight)/double(numPix_y);

            //INICIA
            for(int i=0;i<h;i++)
              for(int j=0;j<w;j++){
                    rect.x=j*numPix_x;
                    rect.y=i*numPix_y;
                    IplImage* sub=subImage(scene,rect);
                    if(spaceColorC1c2c3)
                    Histograma().CalcularHistogramas3Canal(sub,templ_msk_scaled,histo_scene_template,numHistox,numHistoy,nh,ns,nv);
                   else
                     Histograma().CalcularHistogramasHSV(sub,templ_msk_scaled,histo_scene_template,numHistox,numHistoy,nh,ns,nv);
                    //aqui

                     double valorcito=isimi.HSVhistograma(histo_template,histo_scene_template,numHistox*numHistoy);

                     ImageLocation* p1=new ImageLocation(rect.x,rect.y,rect.width,rect.height,0,valorcito,escalaActual);

                     histogramOrdered.push(p1);

                    cvReleaseImage(& sub);
                    for(int l=0;l<numHistox*numHistoy;l++)
                    histo_scene_template[l].release();
                    delete[] histo_scene_template;

                   }//fin ij

            //CAMBIO 3 NO DECREMENTAR
        samples_width+=decrementox;
        samples_Height+=decrementoy;

        cvReleaseImage(& templ_msk_scaled);
}

    pointOut=new ImageLocation*[numOut];
    int countprocessed=0;

    double pix=0.07*scene->width;

    while(!histogramOrdered.empty()){
         bool cercano=false;
             ImageLocation* minLoc_=histogramOrdered.top();
         for(int k=0;k<countprocessed;k++){
             ImageLocation* anterior=pointOut[k];
             if(anterior->isnear(*minLoc_,pix)){
             cercano=true;
             break;
             }

         }//fin k

         if(!cercano){
             pointOut[countprocessed]=minLoc_;
             countprocessed++;
         }
         if(countprocessed>=numOut)
             break;
         histogramOrdered.pop();
        }
    //liberar Memoria de priority queue restante
    while(!histogramOrdered.empty())
        histogramOrdered.pop();
    return true;
}

////define's

bool FindObject::FindByHistogramaXY_define(IplImage *scene, cv::Mat *&histo_template, IplImage *msk_template, ImageLocation **&pointsIn, int numIn, ImageLocation **&pointsOut, int numout, int numHistox, int numHistoy, int nh, int ns, int nv,bool spaceColorC1c2c3){

    priority_queue<ImageLocation*,vector<ImageLocation*>,ComparePoint> valuesHistograma;
ToolsImages ti;
ImageSimilarity is;
Histograma obj_hist;

cv::Mat* histo_scene_template;

   for(int i=0;i<numIn;i++){
       CvRect rect;
       rect.height=pointsIn[i]->height;
       rect.width=pointsIn[i]->width;
       rect.x=pointsIn[i]->x;
       rect.y=pointsIn[i]->y;
       IplImage* sub=subImage(scene,rect);
       IplImage* templ_msk_scaled=ti.EscalarImagen(msk_template,rect.width,rect.height);
       if(spaceColorC1c2c3)
           obj_hist.CalcularHistogramas3Canal(sub,templ_msk_scaled,histo_scene_template,numHistox,numHistoy,nh,ns,nv);
       else
           obj_hist.CalcularHistogramasHSV(sub,templ_msk_scaled,histo_scene_template,numHistox,numHistoy,nh,ns,nv);

           double val=is.HSVhistograma(histo_template, histo_scene_template, numHistox*numHistoy);
        ImageLocation* nuevo=new ImageLocation(rect.x,rect.y,rect.width,rect.height,pointsIn[i]->templateNumber,val,double(rect.height)/double(msk_template->height));
        valuesHistograma.push(nuevo);

        cvReleaseImage(&sub);
        cvReleaseImage(&templ_msk_scaled);
        for(int l=0;l<numHistox*numHistoy;l++)
             histo_scene_template[l].release();
          delete []histo_scene_template;


   }
   pointsOut=new ImageLocation*[numout];
   for(int i=0;i<numout;i++){

        if(!valuesHistograma.empty()){
            ImageLocation* detectado=valuesHistograma.top();
            pointsOut[i]=detectado;
            valuesHistograma.pop();
        }
   }


return true;
}



bool FindObject::FindBySpacial_define(IplImage *scene, IplImage *templ, IplImage *msk_template, ImageLocation **&pointsIn, int numIn, ImageLocation **&pointsOut, int numout, bool spaceColorC1c2c3, int medidaSimilitud, int numCanalColor,int numCuantificacion){
priority_queue<ImageLocation*,vector<ImageLocation*>,ComparePoint> valuesCorrelation;
ToolsImages ti;
ImageSimilarity isimi;


   for(int i=0;i<numIn;i++){
       CvRect rect;
       rect.height=pointsIn[i]->height;
       rect.width=pointsIn[i]->width;
       rect.x=pointsIn[i]->x;
       rect.y=pointsIn[i]->y;
       IplImage* sub=subImage(scene,rect);



       IplImage* templ_scaled=ti.EscalarImagen(templ,rect.width,rect.height);
       IplImage* templ_msk_scaled=ti.EscalarImagen(msk_template,rect.width,rect.height);
       double valorcito=0;
       if(numCanalColor==3){
               switch(medidaSimilitud){
                   case 1:
                       valorcito=isimi.correlationMSK3Canal(sub,templ_scaled,templ_msk_scaled,spaceColorC1c2c3);
                       break;
                   case 2:
                      valorcito=isimi.DiferenciasCuadrado3Canal(sub,templ_scaled,templ_msk_scaled,spaceColorC1c2c3);
                      break;
                  default:
                    valorcito=isimi.SpacialMSK3Canal(sub,templ_scaled,templ_msk_scaled, 0.3,spaceColorC1c2c3);
                    break;
               }}
             else
               switch(medidaSimilitud){
                   case 1:
                       valorcito=isimi.correlationMSK1Canal(sub,templ_scaled,templ_msk_scaled,numCuantificacion,spaceColorC1c2c3);
                       break;
                   case 2:
                      valorcito=isimi.DiferenciasCuadrado1Canal(sub,templ_scaled,templ_msk_scaled,numCuantificacion,spaceColorC1c2c3);
                      break;
                  default:
                    valorcito=isimi.spacialMSK1Canal(sub,templ_scaled,templ_msk_scaled,numCuantificacion, 0.3,spaceColorC1c2c3);
                    break;
         }

       ImageLocation* nuevo=new ImageLocation(rect.x,rect.y,rect.width,rect.height,pointsIn[i]->templateNumber,valorcito,double(rect.height)/double(msk_template->height));
        valuesCorrelation.push(nuevo);

        cvReleaseImage(&sub);
        cvReleaseImage(&templ_scaled);
        cvReleaseImage(&templ_msk_scaled);


   }
   pointsOut=new ImageLocation*[numout];
   for(int i=0;i<numout;i++){

        if(!valuesCorrelation.empty()){
            ImageLocation* detectado=valuesCorrelation.top();
            pointsOut[i]=detectado;
            valuesCorrelation.pop();
        }
   }


return true;
}


bool FindObject::FindBySURF_define(IplImage *scene, CvSeq *objectKeyPoint, CvSeq *objectDescriptors, ImageLocation **&pointsIn, int numIn, ImageLocation *&pointsOut){
    ToolsImages ti;
    priority_queue<ImageLocation*,vector<ImageLocation*>,ComparePoint> valuessurf;

 CvSeq *scenetKeyPointC;
 CvSeq *sceneDescriptorsC;


 ti.getKeyPoints(scene,&scenetKeyPointC,&sceneDescriptorsC);


   for(int i=0;i<numIn;i++){
       CvRect rect;
       rect.height=pointsIn[i]->height;
       rect.width=pointsIn[i]->width;
       rect.x=pointsIn[i]->x;
       rect.y=pointsIn[i]->y;
     //  IplImage* sub=subImage(scene,rect);

       //obtener puntos surf de sub
       vector<int> indicesScene;
       MatchingSURF().pointsInRect(scenetKeyPointC,sceneDescriptorsC,indicesScene, rect);
       double valorcito=MatchingSURF().findHomography(objectKeyPoint,objectDescriptors,scenetKeyPointC, sceneDescriptorsC,indicesScene);
     //  if(valorcito)
       ImageLocation* nuevo=new ImageLocation(rect.x,rect.y,rect.width,rect.height,pointsIn[i]->templateNumber,valorcito,pointsIn[i]->escala);
        valuessurf.push(nuevo);

        }

   ImageLocation* detectado=valuessurf.top();
   bool si=false;
   if(detectado->value>0){
       pointsOut=detectado;
       si=true;
}


while(!valuessurf.empty()){
    detectado=valuessurf.top();
    valuessurf.pop();
}

return si;
}


bool FindObject::FindBySURF_define(CvSeq *sceneKeyPoint, CvSeq *sceneDescriptors, CvSeq *objectKeyPoint, CvSeq *objectDescriptors, ImageLocation **&pointsIn, int numIn, ImageLocation *&pointOut) {

    priority_queue<ImageLocation*,vector<ImageLocation*>,ComparePoint> valuessurf;



   for(int i=0;i<numIn;i++){
       CvRect rect;
       rect.height=pointsIn[i]->height;
       rect.width=pointsIn[i]->width;
       rect.x=pointsIn[i]->x;
       rect.y=pointsIn[i]->y;
     //  IplImage* sub=subImage(scene,rect);

       //obtener puntos surf de sub
       vector<int> indicesScene;
       MatchingSURF().pointsInRect(sceneKeyPoint,sceneDescriptors,indicesScene, rect);


       double valorcito=MatchingSURF().findHomography(objectKeyPoint,objectDescriptors,sceneKeyPoint, sceneDescriptors,indicesScene);
     //  if(valorcito)
       ImageLocation* nuevo=new ImageLocation(rect.x,rect.y,rect.width,rect.height,pointsIn[i]->templateNumber,valorcito,pointsIn[i]->escala);
        valuessurf.push(nuevo);

        }

   ImageLocation* detectado=valuessurf.top();
   bool si=false;
   if(detectado->value>0.1){
       pointOut=detectado;
       si=true;
}


while(!valuessurf.empty()){
    detectado=valuessurf.top();
    valuessurf.pop();
}

return si;
}



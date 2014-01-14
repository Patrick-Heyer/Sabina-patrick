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

#include "../Plugin_API/pluginapi.h"
#include "../Shared_memory/Robot.h"
#include "../MDP/MDP.h"
#include "../Gui/gui.h"
#include "../Gui/console.h"
#include "../Gui/video.h"

#include "surfrecognizer.h"

Robot *patrol;
Video *videoDisplay;
Tab *pluginTab;

#define PLUGIN_NAME "Vision_Surf"

/**
 * @brief Vision_Surf plugin
 *
 * This plugin is used for object recognition based on OpenCV's basic SURF example
 * @author     Patrick Heyer, patrickhey@prodigy.net.mx, Irving V&aacute;squez ivasquez@inaoep.mx
 * @date       jul 04, 2012
 * @version    1.0
 **/
class Vision_Surf : public IPlugin
{
public:
    void Main();
    void stop();
    void run();
};

PLUGIN_FUNC IPlugin *CreatePlugin()
{
    return new Vision_Surf;
}

PLUGIN_FUNC void DestroyPlugin(IPlugin *r)
{
    delete r;
}

PLUGIN_DISPLAY_NAME(PLUGIN_NAME);

PLUGIN_INIT()
{
    // register our new plugin

    RegisterPlugin(PLUGIN_NAME, CreatePlugin, DestroyPlugin);
    return 0;
}

cv::Mat H;

void createHomografy() {
    //Formato Es: {ROW, COL, UNO}
    //Webcam
  
  //PATRICK
    float P11[] = {156,  142,  1}; // Left Sup
    float P12[] = {155,  470, 1}; // Right Sup
    float P13[] = {452, 616, 1}; // Right Inf
    float P14[] = {463, 18,  1}; // Left Inf

    //KINECT
    float P21[] = {12, 191}; // left sup
    float P22[] = {3, 445}; // right sup
    float P23[] = {239, 499}; // right inf
    float P24[] = {251, 171}; // left inf
    
    
    
      //PATRICK backup homogrpy kinect arriba
//     float P11[] = {165,  152,  1}; // Left Sup
//     float P12[] = {157,  476, 1}; // Right Sup
//     float P13[] = {46, 615, 1}; // Right Inf
//     float P14[] = {453, 12,  1}; // Left Inf
// 
//     //KINECT
//     float P21[] = {104, 219}; // left sup
//     float P22[] = {100, 416}; // right sup
//     float P23[] = {287, 447}; // right inf
//     float P24[] = {291, 189}; // left inf

    float A1_1 = P11[0];
    float A1_2 = P11[1];
    float A1_3 = P11[2];
    float A2_4 = P11[0];
    float A2_5 = P11[1];
    float A2_6 = P11[2];
    float A1_7 = -P21[0] * P11[0];
    float A1_8 = -P21[0] * P11[1];
    float A2_7 = -P21[1] * P11[0];
    float A2_8 = -P21[1] * P11[1];
    float A1_9 = -P21[0];
    float A2_9 = -P21[1];

    float A3_1 = P12[0];
    float A3_2 = P12[1];
    float A3_3 = P12[2];
    float A4_4 = P12[0];
    float A4_5 = P12[1];
    float A4_6 = P12[2];
    float A3_7 = -P22[0] * P12[0];
    float A3_8 = -P22[0] * P12[1];
    float A4_7 = -P22[1] * P12[0];
    float A4_8 = -P22[1] * P12[1];
    float A3_9 = -P22[0];
    float A4_9 = -P22[1];

    float A5_1 = P13[0];
    float A5_2 = P13[1];
    float A5_3 = P13[2];
    float A6_4 = P13[0];
    float A6_5 = P13[1];
    float A6_6 = P13[2];
    float A5_7 = -P23[0] * P13[0];
    float A5_8 = -P23[0] * P13[1];
    float A6_7 = -P23[1] * P13[0];
    float A6_8 = -P23[1] * P13[1];
    float A5_9 = -P23[0];
    float A6_9 = -P23[1];

    float A7_1 = P14[0];
    float A7_2 = P14[1];
    float A7_3 = P14[2];
    float A8_4 = P14[0];
    float A8_5 = P14[1];
    float A8_6 = P14[2];
    float A7_7 = -P24[0] * P14[0];
    float A7_8 = -P24[0] * P14[1];
    float A8_7 = -P24[1] * P14[0];
    float A8_8 = -P24[1] * P14[1];
    float A7_9 = -P24[0];
    float A8_9 = -P24[1];
    //cout<<"NAAAAADAAAA 000000000"<<endl;
    float T[] = //8 Renglones y 9 columnas
    {   A1_1, A1_2, A1_3, 0.0 , 0.0 , 0.0 , A1_7, A1_8, A1_9,
        0.0 , 0.0 , 0.0 , A2_4, A2_5, A2_6, A2_7, A2_8, A2_9,
        A3_1, A3_2, A3_3, 0.0 , 0.0 , 0.0 , A3_7, A3_8, A3_9,
        0.0 , 0.0 , 0.0 , A4_4, A4_5, A4_6, A4_7, A4_8, A4_9,
        A5_1, A5_2, A5_3, 0.0 , 0.0 , 0.0 , A5_7, A5_8, A5_9,
        0.0 , 0   , 0.0 , A6_4, A6_5, A6_6, A6_7, A6_8, A6_9,
        A7_1, A7_2, A7_3, 0.0 , 0.0 , 0.0 , A7_7, A7_8, A7_9,
        0.0 , 0.0 , 0.0 , A8_4, A8_5, A8_6, A8_7, A8_8, A8_9
    };


    cv::Mat A = cv::Mat(8,9, CV_32F, T).clone();
    cv::Mat B = A.t() * A; //cv::Matriz simétrica

    cv::Mat V; // eigen vectores
    cv::Mat D ;// eigen valores.
    eigen(B,D,V);
    float t_H[] = {
        V.at<float>(8,0), V.at<float>(8,1), V.at<float>(8,2),
        V.at<float>(8,3), V.at<float>(8,4), V.at<float>(8,5),
        V.at<float>(8,6), V.at<float>(8,7), V.at<float>(8,8),
    };
    H = cv::Mat(3,3, CV_32F, t_H).clone();
    //cout<<"H tiene: "<<endl<<H<<endl;

    // Al final H debe ser un DATO MIEMBRO DE LA CLASE.
    // todo LO EJECUTADO ARRIBA, SOLO DEBE EJECUTARSE UNA UNICA VEZ. y lo UNICO
    // que DEBE GUARDARSE es H.

    //Argumentos: RenglonDeKinect, ColumnaDeKinect, Renglon de Camarita, ColumnadeCamarita.
//     int R, C;
//     TransformarPunto(614, 752, R, C, H);
//     TransformarPunto(941, 1043, R, C, H);
//     cout<< "Salida es Row:"<< R << " y Col:"<<C <<endl;



    // %H Calculation,  P1 H = P2 // Solution Ax=0
    // %Solving the system
}


void TransformarPunto(int Row, int Col, int& OutPutRow, int& OutPutCol, cv::Mat &H) {
    float Punto[] = {(float)Row, (float)Col, 1.0};
    cv::Mat P = cv::Mat(3, 1, CV_32F, Punto).clone();
    //cout <<"P TIENE: "<<endl<<P<<endl;
    //cout <<" H " << H.rows << " " << H.cols << " P " << P.rows << " " << P.cols << endl;
    cv::Mat NewPoint = H * P; //No necesita la traspuesta porque se declaro a P como vector columna de 3filas x 1columna.
    //cout <<" Final Tiene"<<endl<<NewPoint<<endl;
    OutPutRow = (int) (NewPoint.at<float>(0,0) / NewPoint.at<float>(2,0));
    OutPutCol = (int) (NewPoint.at<float>(1,0) / NewPoint.at<float>(2,0));
}


void Vision_Surf::Main()
{
    /**
     * \sa cambiar_estado
     **/

    Gui::getInstance();
    IplImage *frame;
    pluginTab = new Tab("Vision_Surf");
    videoDisplay = new Video(0,HEIGHT*.02,WIDTH/2,HEIGHT/2,"Vision_Surf", pluginTab);
    
    createHomografy();


    //    SurfRecognizer recognizer2;
    //    SurfRecognizer recognizer3;

    // string objeto_file("box.png");





    std::string accion;

    for (;;)
    {
        accion=patrol->getInstance().get_Action();
        if (accion=="reconocer_objeto")
        {
            std::stringstream ss;
            ss << "../data/Surf/" << patrol->getInstance().Microphone.get_Phrase() << ".png";
            string objeto_file1(ss.str());
            SurfRecognizer recognizer;
            recognizer.setObjectFileName(objeto_file1.c_str());
            recognizer.setDisplaying(true);
            recognizer.setRecognitionTreshold(3);
            recognizer.loadObjectFile();

	    int x,y;
            do
            {
                frame=patrol->getInstance().Ptz.get_image();
                videoDisplay->SetImage(frame);

            } while(!recognizer.recongnizeObjectInImage(frame,x,y));
            
            ss.str("");
            ss << "I found " << patrol->getInstance().Microphone.get_Phrase();
	    std::cout << "pinche objeto esta en " << x << " " << y << std::endl;
	    
	     TransformarPunto(y,x, patrol->getInstance().object_Y, patrol->getInstance().object_X,H);
// 	    TransformarPunto(x,y, patrol->getInstance().object_X, patrol->getInstance().object_Y,H);
	    std::cout << "el kinect lo ve en  " << patrol->getInstance().object_X << " " << patrol->getInstance().object_Y << std::endl;
	    
            patrol->getInstance().Sintetizer.set_Phrase(ss.str().c_str());
            sleep(3);
            patrol->getInstance().set_Action(cambiar_estado("objeto_reconocido", "si"));
            sleep(2);
            cvReleaseImage(&frame);


        }
    }

}

void Vision_Surf::run()
{
    pthread_create(&thread_id, NULL, &IPlugin::IncWrapper, this);
}

void Vision_Surf::stop()
{

}


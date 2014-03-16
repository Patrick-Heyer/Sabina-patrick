// g++ `pkg-config --cflags opencv` -o HC HC.cpp `pkg-config --libs opencv`
#include "HC.hpp"

HC::HC() {
//inicio Parametros:
    UMBRAL_MATAR_HUERFANOS = 40;
    UMB_DIST = 80; //Umbral: Criterio de Paro para el k-means jerarquico.
    VistaRows  =  10; //Promedio de Altura de las imágenes "Vista"
    VistaCols  =  10; //Promedio de Anchura de las imágenes "Vista"
    ImgRows    = 480; // Alto de la imagen.
    ImgCols    = 640; // Ancho de la imagen.
    UMB_NCLUST = 8; //Número mínimo de elementos que debe tener el cluster para ser convertido a ventana.
}
// Fin Parametros.

HC::HC(int Rows, int Cols) {
//inicio Parametros:
    UMBRAL_MATAR_HUERFANOS = 60;
    UMB_DIST = 80; //Umbral: Criterio de Paro para el k-means jerarquico.
    VistaRows  =  15.0; //Promedio de Altura de las imágenes "Vista"
    VistaCols  =  15.0; //Promedio de Anchura de las imágenes "Vista"
    ImgRows    = Rows; // Alto de la imagen.
    ImgCols    = Cols; // Ancho de la imagen.
    UMB_NCLUST = 20.0; //Número mínimo de elementos que debe tener el cluster para ser convertido a ventana.
}
HC::~HC() {
}

Mat HC::Execute(vector<KeyPoint>& KeyPuntos, Mat& Ventanas, Mat& Centros_OUT) {
    cout<<"Preparing Hie. Clust. Data"<<endl;
    KeyPoint* p;
    Mat Puntos(KeyPuntos.size(), 2, CV_32F);
    for(int i = 0; i < KeyPuntos.size(); i++)
    {
        p = &KeyPuntos[i];
        Puntos.at<float>(i, 0) = p->pt.y;
        Puntos.at<float>(i, 1) = p->pt.x;
        //cout<<"Ejemplo: "<<Puntos.at<float>(i, 0)<<endl; //se eliminará.
    }

    int N_points = Puntos.rows;
    Mat MDists(N_points, N_points, CV_32F); // ¿y si uso CV_64F?
//float ix, iy;
// float jx, jy;
    Mat Diff;
    for(int i = 0; i < N_points; i++) {
        //ix = Puntos.at<float>(i,0);
        //iy = Puntos.at<float>(i,1);
        for(int j = 0; j < N_points; j++) {
            if(i < j) { //Produce una matriz triangular superior.
                //jx = Puntos.at<float>(j,0);
                //jy = Puntos.at<float>(j,1);
                Diff = Puntos.row(i) - Puntos.row(j);
                MDists.at<float>(i,j) = sqrt(Diff.dot(Diff)); //guarda la distancia en la MAT.
            }// Fin del IF
        } // fin del j-FOR
    } // fin del i-FOR


//Rellenar la matriz (el triangulo inferior)
    for(int i = 0; i < N_points; i++) {
        for(int j = 0; j < N_points; j++) {
            if(i < j) { //Produce una matriz triangular Inferior
                MDists.at<float>(j,i) = MDists.at<float>(i,j);
            }// Fin del IF
        } // fin del j-FOR
    } // fin del i-FOR

//Rellenar la diagonal.
// for(int i = 0; i < N_points; i++){
//   MDists.at<float>(i,i) = 0; // llena de ceros la diagonal principal de la matriz. (Evita qe tenga basura).
// } //Fin i-FOR

    double N_min, N_max;
    Point Pmin, Pmax;

// rellenar la diagonal con un numero muy alto.
    minMaxLoc(MDists, &N_min, &N_max, &Pmin, &Pmax);
    float Infinite = 3 * N_max;
    for(int i = 0; i < N_points; i++)
    {
        MDists.at<float>(i,i) = Infinite; // la idea es ponerle +Inf
    }

//Etapa de Preproceso

//Ahora hay que eliminar las filas y Columnas donde haya puntos huérfanos.
    Mat Renglon(1, N_points, CV_32F);
    int P_candidatos[N_points];
    int HowManyPoints = 0;
    for(int i = 0; i < N_points; i++)
    {   // Eliminar renglones de puntos huérfanos. (puntos aislados de otros puntos SIFT).
        Renglon = MDists.row(i);
        minMaxLoc(Renglon, &N_min, &N_max, &Pmin, &Pmax);
        // cout <<N_min<< ", ";
        if(N_min > UMBRAL_MATAR_HUERFANOS)
        {   //Si el punto vecino más cercano a este punto es más lejano que el umbral...
            P_candidatos[i] = 0; // Eliminar este punto, pues se trata de un punto huérfano. con pocos puntos como vecinos.
        } else {
            P_candidatos[i] = 1; //Es un punto candidato.
            HowManyPoints++;
        }
    }

// Compactar la matriz para que no tenga ya los puntos huérfanos.
    Mat CompactAux   (HowManyPoints, N_points, CV_32F);
    Mat CompactMatrix(HowManyPoints, HowManyPoints, CV_32F);
    Mat CompactPoints(HowManyPoints, 2, CV_32F);
    for(int i = 0, C = 0; i < N_points; i++)
    {
        if(P_candidatos[i])
        {
            MDists.row(i).copyTo(CompactAux.row(C++)); // Va adquiriendo los renglones correctos.
        }
    }

    for(int i = 0, C = 0; i < N_points; i++)
    {
        if(P_candidatos[i])
        {
            CompactAux.col(i).copyTo(CompactMatrix.col(C)); // Va adquiriendo las columnas correctas.
            Puntos.row(i).copyTo(CompactPoints.row(C++));
        }
    }

    int Etiquetas[HowManyPoints]; //Vector de etiquetas, dice a que cluster pertenecen.
    Mat ScoreElements(HowManyPoints, 1, CV_32F, Scalar(1));
    Mat MinWinClust = CompactPoints;
    Mat MaxWinClust = CompactPoints;
    for(int i = 0; i < HowManyPoints; i++)
    {
        Etiquetas[i]     = i; //inicialmente hay "HowManyPoints" clústeres.
        //ScoreElements[i] = 1; //inicialmente cada cluster tiene 1 elemento.
    }

    /* 1. Calcula la distancia minima. en toda la matriz.
     * 2. Crea un vector de Labels del tamaño de la "HowManyPoints" que dirán cual punto es en la matriz.
     * 3. WHILE: Pregunta si esta dist. es menor al umbral. Si sí continua, si no, ahi muere.
     * 3. Crea un nuevo punto que es el centroide de estos dos puntos (coords x & y) (cuidado, quizá estén cruzados)
     * 4. Etiqueta el vector Etiquetas[] a estos dos puntos
     * 5. Elimina los 2 puntos de la matriz y crea una nueva matriz eliminando las 2 filas y las 2 columnas.
     * 6. Crea un Vector de Labels que dice cual es el nombre original de los elementos de esta nueva matriz.
     * 7. Saca las distancias de este punto con respecto de los demás.
     * */

    Mat Salida;
    Mat CENTROS;
    cout<<"Kmeans..."<<endl;
    double XX = kmeans(CompactPoints, 7,Salida, TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, 0.0001), 5, cv::KMEANS_RANDOM_CENTERS, CENTROS );
    cout<<"TAMAÑO: "<<CENTROS.rows<<" y COLS: "<<CENTROS.cols<<endl;

    cout<<"HC Local"<<endl;
//cvflann::KMeansIndexParams k_params(10, 1000, cvflann::FLANN_CENTERS_KMEANSPP,0.01);
//Mat1f Zenters(10,2);
//Zenters.setTo(0);
// int count = cvflann::hierarchicalClustering<cv::flann::L2<float> >(CompactPoints, Zenters, k_params);
//cout << Zenters << endl;
//cout<<"...Fin Flann HC."<<endl;



    Centros_OUT = CENTROS;
    cout<<"Hierarchical Clustering..."<<endl;
    minMaxLoc(CompactMatrix, &N_min, &N_max, &Pmin, &Pmax); // 1.
    double DistCalculada = N_min;
    int Px, Py, RealX, RealY;
    float NewPoints[HowManyPoints][2];
    float NPROW, NPCOL;
    int P = 0;
    float MinY0, MinX0, MinY1, MinX1;
    float MaxY0, MaxX0, MaxY1, MaxX1;
    float maxROW, maxCOL;
    float minROW, minCOL;
// cout << "Publicamos CompactPoints: " << CompactPoints<< endl;
//cout<<"Public MinWclust: "<<MinWinClust.row(1)<<" y Max es: "<<MaxWinClust.row(1)<<endl;
    while(DistCalculada < UMB_DIST)
    {   // 3
        Px = Pmin.x;
        Py = Pmin.y;
        //cout<<"Px y Py valen: "<<Px<<", "<<Py<<endl;
        //cout<<"Public MinWclust: "<<MinWinClust.row(Px)<<" y Max es: "<<MinWinClust.row(Py)<<endl;
        MinX0 = MinWinClust.at<float>(Px, 0);
        MinY0 = MinWinClust.at<float>(Py, 0);
        MinX1 = MinWinClust.at<float>(Px, 1);
        MinY1 = MinWinClust.at<float>(Py, 1);

        MaxX0 = MaxWinClust.at<float>(Px, 0);
        MaxY0 = MaxWinClust.at<float>(Py, 0);
        MaxX1 = MaxWinClust.at<float>(Px, 1);
        MaxY1 = MaxWinClust.at<float>(Py, 1);

        if(MinX0 < MinY0)
            minROW = MinX0;
        else
            minROW = MinY0;

        if(MinX1 < MinY1)
            minCOL = MinX1;
        else
            minCOL = MinY1;


        if(MaxX0 > MaxY0)
            maxROW = MaxX0;
        else
            maxROW = MaxY0;

        if(MaxX1 > MaxY1)
            maxCOL = MaxX1;
        else
            maxCOL = MaxY1;
        //cout<<"Valores Min Max son: "<<minROW<<", "<<minCOL<<" y MAX son: "<<maxROW<<", "<<maxCOL<<endl;

        NPROW = (CompactPoints.at<float>(Px, 0) + CompactPoints.at<float>(Py, 0) ) / 2.0; //fusionando coordenadas.
        NPCOL = (CompactPoints.at<float>(Px, 1) + CompactPoints.at<float>(Py, 1) ) / 2.0; //Fusionando coordenadas.
        // cout<< "Nuevo Punto es: " << NPROW << " y " << NPCOL << endl;
        // cout<< "Coordenadas son: " << Px << " y " << Py << endl;
        CompactMatrix = QuitarPuntos      (CompactMatrix, Px, Py, CompactPoints); // Quita los puntos de la mat de dist. y de la mat. de centros.
        ScoreElements = RecaulcularScore  (ScoreElements, Px, Py); //quita los 2 elementos, y sus valores los suma al final. (para saber cuantos elementos tiene cada cluster.
        RecalcularMinMaxRectangle(MinWinClust, MaxWinClust, Px, Py, minROW, minCOL, maxROW, maxCOL);
        CompactMatrix = RecalcDistNewPoint(CompactMatrix, NPROW, NPCOL, CompactPoints, Infinite); // 7
        minMaxLoc(CompactMatrix, &DistCalculada, &N_max, &Pmin, &Pmax); // Repetir....
        // Se sigue el ciclo hasta salirse.
        // cout<< "la distancia minima ahora es: " << DistCalculada<< endl;
    }


// 1. Ahora preguntar cuantos clusteres superan el umbral.
//cout<< "Publicamos Tamaños: " << MinWinClust.row(1)<< " y Max: "<<MaxWinClust.row(1)<<endl;

    int N_Clusteres = CompactMatrix.rows;
    cout << "Salieron "<< N_Clusteres << " Clusteres" << endl;

    int rowini, rowend, colini, colend;
    double t;
    Mat VentanasAux(N_Clusteres, 4, CV_32F);
    int Puntero = 0 ; //Significa Vacio.

    for(int i = 0; i < N_Clusteres; i++) {
        //cout<< "Score: "<<ScoreElements.at<float>(i) <<" y Clust: " <<UMB_NCLUST<<endl;
        if(ScoreElements.at<float>(i) > UMB_NCLUST) {
            //crear las coordenadas de la ventana.
            rowini = (t = MinWinClust.at<float>(i,0) - VistaRows) < 0 ? 0: (int)t;
            rowend = (t = MaxWinClust.at<float>(i,0) + VistaRows) > ImgRows ? ImgRows: (int)t;
            colini = (t = MinWinClust.at<float>(i,1) - VistaCols) < 0 ? 0: (int)t;
            colend = (t = MaxWinClust.at<float>(i,1) + VistaCols) > ImgCols ? ImgCols: (int)t;
            AddWindow(VentanasAux, rowini, rowend, colini, colend, ++Puntero);

        } else {
            //No crear ventana.
        }
    }

    Ventanas = CompactVentanas(VentanasAux, Puntero); //las pone como debe de ser.
    cout << "Salieron "<< Puntero << " Ventanas"<<endl;
//cout << "las coordenadas de las ventanas son: "<<endl<< Ventanas<< endl;

    return CompactPoints;
// lo que devuelve es Ventanas, una MAT de N x 4 que contiene las coordenadas de las ventanas
// ORDEN: ROWINI, ROWFINAL, COLUMNAINI, COLUMNAFINAL
}

Mat HC::QuitarPuntos(Mat& CompactMatrix, int Pcol, int Prow, Mat &CompactPoints)
{
    // Quita los puntos tanto de la matriz de distancias, como de la matriz de puntos (centros).
    int N = CompactPoints.rows;
    Mat NewCompact(N - 2, 2, CV_32F);
    Mat NewMatrix(N - 2, N -2, CV_32F);
    Mat T1(N - 2, N, CV_32F);
    // cout << "deben ser coincidir : " << CompactMatrix.cols << " y " << CompactMatrix.rows << " y " << CompactPoints.rows << endl;
    for(int i = 0, C = 0; i < N; i++)
    {
        if((i != Pcol) && (i != Prow))
        {   // Si No es un punto a eliminar, (RECUERDE LEYES DE DE MORGAN
            CompactPoints.row(i).copyTo(NewCompact.row(C));
            CompactMatrix.row(i).copyTo(T1.row(C++));
        }
    }
    for(int i = 0, C = 0; i < N; i++)
    {
        if((i != Pcol) && (i != Prow))
        {   // Compacta las columnas de la Matriz de distancias.
            T1.col(i).copyTo(NewMatrix.col(C++));
        }
    }
    CompactPoints = NewCompact; //sustituye los puntos.
    return NewMatrix; //sustituye la matriz de distnacias.
}

Mat HC::RecaulcularScore(Mat& ScoreElements, int Pcol, int Prow)
{
    int N = ScoreElements.rows;
    Mat NewMatScore(N - 1, 1, CV_32F);
    for(int i = 0, C = 0; i < N; i++)
    {
        if((i != Pcol) && (i != Prow))
        {   // Compacta el vector de Scores.
            NewMatScore.at<float>(C++) = ScoreElements.at<float>(i);
        }
    }
    //cout<< "Sumandos son: " << ScoreElements.at<float>(Pcol) <<" y " <<ScoreElements.at<float>(Prow)<< endl;
    NewMatScore.at<float>(N - 2) = ScoreElements.at<float>(Pcol) + ScoreElements.at<float>(Prow);
    //quita los 2 elementos, y sus valores los suma al final. (para saber cuantos elementos tiene cada cluster.
    return NewMatScore;
    //cout <<"Vector de Scores en cada iteracion: "<< NewMatScore<< endl;

}

void HC::RecalcularMinMaxRectangle(Mat& MinWinClust, Mat& MaxWinClust, int Pcol,int Prow, float& minROW, float& minCOL, float& maxROW, float& maxCOL)
{
    int N = MinWinClust.rows;
    Mat NewMinWinClust(N - 1, 2, CV_32F);
    Mat NewMaxWinClust(N - 1, 2, CV_32F);

    for(int i = 0, C = 0; i<N; i++)
    {
        if((i != Pcol) && (i != Prow))
        {   // Compacta el vector de Min y Max.
            MinWinClust.row(i).copyTo(NewMinWinClust.row(C)); // Va adquiriendo los renglones correctos.
            MaxWinClust.row(i).copyTo(NewMaxWinClust.row(C++));
        }
    }
    NewMinWinClust.at<float>(N - 2, 0) = minROW;
    NewMinWinClust.at<float>(N - 2, 1) = minCOL;
    NewMaxWinClust.at<float>(N - 2, 0) = maxROW;
    NewMaxWinClust.at<float>(N - 2, 1) = maxCOL;
    MinWinClust = NewMinWinClust;
    MaxWinClust = NewMaxWinClust;

}

Mat HC::RecalcDistNewPoint(Mat& CompactMatrix, float NewProw, float NewPcol, Mat& CompactPoints, float& Infinite)
{
    // 7. Saca las distancias de este punto con respecto de los demás.
    // 1. Voy a agregar los nuevos puntos a CompactPoints
    // 2. Voy a añadir una fila y una columna a CompactMatrix y la voy a devolver en la funcion.

    int r = CompactPoints.rows;

    Mat NewC; // (r + 1, 2,CV_32F);
    Mat T1(1, 2, CV_32F);
    vconcat(CompactPoints ,T1, NewC);
    r = NewC.rows;

    NewC.at<float>(r - 1, 0) = NewProw;
    NewC.at<float>(r - 1, 1) = NewPcol;
    Mat Diff;
    float Dist;
    CompactPoints = NewC;
    Mat LastRenglon(1, r - 1, CV_32F);
    for(int i = 0; i < (r - 1); i++) {
        Diff = NewC.row(i) - NewC.row(r - 1);
        Dist = sqrt( Diff.dot(Diff));
        LastRenglon.at<float>(0, i) = Dist;
    }
    Mat NewMatrix, Temp1;


    vconcat(CompactMatrix, LastRenglon, Temp1);
    Mat LastColumn = LastRenglon.t();
    Mat T2(1,1,CV_32F);
    vconcat(LastColumn, T2, LastColumn);
    LastColumn.at<float>(r - 1 , 0) = Infinite;
    hconcat(Temp1, LastColumn, NewMatrix);

    return NewMatrix;
}

void HC::AddWindow(Mat& Ventanas, int& rowini, int& rowend, int& colini, int& colend, int Puntero)
{
    Ventanas.at<float>(Puntero - 1, 0) = (float) rowini;
    Ventanas.at<float>(Puntero - 1, 1) = (float) rowend;
    Ventanas.at<float>(Puntero - 1, 2) = (float) colini;
    Ventanas.at<float>(Puntero - 1, 3) = (float) colend;
}

Mat HC::CompactVentanas(Mat CompactV, int p)
{
    // Parece una funcion lenta...
    Mat NewWindow(p, 4, CV_32F);
    for(int i = 0; i < p; i++) {
        CompactV.row(i).copyTo(NewWindow.row(i));
    }
    return NewWindow;
}




















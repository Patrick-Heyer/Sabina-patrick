#include "CProbot.h"

#include "Constants.h"
#include "CNearNode.h"
#include "uu_Utils.h"

#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <algorithm>


#define Infin 100000.0

///////////////////////////////////////////////////////////////////////
////////////////////// local function  ////////////////////////////////

bool DistanceCompare( const NearNode &n1, const NearNode &n2 )
// Returns -1 if node n1 is nearer, and +1 if node n2 is nearer.
{
    return ( n1.Distance() < n2.Distance() ) ;
}



CProbot::CProbot()
{
    Ambiente_p_m = new CAmbiente;
    Samples_p_m = new CSamples ;
    Sampler_p_m = NULL ;
    LocalPlanner_p_m = NULL;
    Graph_p_m = new uu_Graph(this);
    
    Parametros_m.ElapsedTime = 0 ;
    Counters.clear() ;
    
    for ( int i=0 ; i<PRM::NR_OF_COUNTERS ; i++ ) Counters.push_back(0) ;
    
    Parametros_m.Sigma = Ambiente_p_m->sizeRobot();
    
}


CProbot::~CProbot()
{
    
    if (Ambiente_p_m != NULL)
    {
        delete Ambiente_p_m;
        Ambiente_p_m = NULL;
    }
    if (Samples_p_m != NULL)
    {
        delete Samples_p_m ;
        Samples_p_m = NULL;
    }
    if (Sampler_p_m != NULL)
    {
        delete Sampler_p_m ;
        Sampler_p_m = NULL;
    }
    if (LocalPlanner_p_m != NULL)
    {
        delete LocalPlanner_p_m ;
        LocalPlanner_p_m = NULL;
    }
    if (Graph_p_m != NULL)
    {
        delete Graph_p_m ;
        Graph_p_m = NULL;
    }
}



int CProbot::BuildRoadMap()
{
    CPersonajeConfig NewConfig ;
    int NewNode ;
    std::vector<NearNode> Neighbours ;
    
    
    Parametros_m.BeginTime = clock() ;
    
    for (unsigned int i=0;i<Counters.size();i++) Counters[i] = 0 ;
    
    if (Parametros_m.SamplingMethod == PRM::RANDOM_S)
    {
        //    SetSampler( (uu_Sampler *)(new uu_UniformRandomSampler(this)) ) ;     //HUSSEIN
        //    printf("\nSe va a hacer UniformRandomSampler\n");                     //HUSSEIN
        
        SetSampler( (uu_Sampler *)(new uu_GaussianSampler(this)) );         //HUSSEIN
        printf("\nSe va a hacer GaussianSampler\n");                        //HUSSEIN
        
    }
    else if (Parametros_m.SamplingMethod == PRM::GAUSSIAN_S )
    {
        SetSampler( (uu_Sampler *)(new uu_GaussianSampler(this)) );
        
    }
    else
        cout<<"PRM: Método de muestreo elegido no existe"<<endl;
    
    
    switch(Parametros_m.Local )
    {
        /*    case PRM::STRAIGHT_LP:
         *    {
         *      SetLocalPlanner( new StraightLinePlanner(this) ) ;
         *      break ;
         }
        case PRM::POTFIELD_LP:
        {
            SetLocalPlanner( new PotentialFieldPlanner(this) ) ;
            break ;
        }*/
        case PRM::TRANSLATE_LP:
        {
            SetLocalPlanner( new TranslateOnlyPlanner(this) ) ;
            break ;
        }
    }
    
    if ( LocalPlanner_p_m == NULL) cout<<"No local planner"<<endl ;
    
    while ( (!RoadMapReady()) && (!(Parametros_m.Interrupt)) )
    {
        // make sure a succesful sample is generated
        while ( !Sampler_p_m->Generate( NewConfig ) && ((!RoadMapReady()) && (!(Parametros_m.Interrupt))) )
        {
        }
        NewNode = Graph_p_m->AddNode( NewConfig ) ;  // also takes care of visualisation
        
        ComputeNeighbours( Neighbours, NewNode ) ;    //calculamos vecinos
        
        AddEdges( NewNode, Neighbours ) ;
        
        // END of TEST
    }
    
    return 1 ;
}


int CProbot::RoadMapReady()
{
    Parametros_m.ElapsedTime = ( clock() - Parametros_m.BeginTime ) ;
    switch ( Parametros_m.Method )
    {
        case PRM::RUN_NODES :
        {
            return ( ( Graph_p_m->NodeNumber() - Counters[PRM::BEGIN_NR_NODES] )
            >=  Parametros_m.NodeNumber ) ;
        }
        case PRM::RUN_TIME :
        {
            return ( Parametros_m.ElapsedTime > Parametros_m.RunTime ) ;
        }
        case PRM::RUN_CONNECT :
        {
            return ( Graph_p_m->Connected( Graph_p_m->StartNode(),
                                           Graph_p_m->GoalNode() ) ) ;
        }
        default :
        {
            return 1 ;
        }
    }
}


void CProbot::Limpiar()
{
    if (Ambiente_p_m != NULL)  Ambiente_p_m->Limpiar();
    if (Samples_p_m != NULL )  Samples_p_m->Clear();
    if (Graph_p_m != NULL )  Graph_p_m->Clear();
    if (LocalPlanner_p_m != NULL ) LocalPlanner_p_m->Clear();
}

void CProbot::setPlanningParametros( const uu_PlanningParameters & Parametros_a )
{
    Parametros_m = Parametros_a ;
}


void CProbot::SetSamples(const CSamples * Samples_p_a)
{
    if (Samples_p_m != NULL)
        delete Samples_p_m ;
    
    Samples_p_m = (CSamples *) Samples_p_a;
}


void CProbot::SetGraph(const uu_Graph * Graph_p_a)
{
    if (Graph_p_m != NULL)
        delete Graph_p_m;
    
    Graph_p_m = (uu_Graph *)Graph_p_a;
    
}


void CProbot::SetLocalPlanner(const uu_LocalPlanner * LocalPlanner_p_a)
{
    if (LocalPlanner_p_m != NULL)
        delete LocalPlanner_p_m;
    
    LocalPlanner_p_m = (uu_LocalPlanner *)LocalPlanner_p_a;
    if (LocalPlanner_p_m!=NULL)
    {
        LocalPlanner_p_m->Count = 0 ;
        LocalPlanner_p_m->CCCount = 0 ;
    }
}

void CProbot::SetSampler(const uu_Sampler * Sampler_p_a)
{
    if (Sampler_p_m != NULL)
        delete Sampler_p_m ;
    
    Sampler_p_m = (uu_Sampler *)Sampler_p_a ;
}

const uu_Graph * CProbot::Graph()const
{
    return Graph_p_m;
}


const uu_LocalPlanner * CProbot::LocalPlanner()const
{
    return LocalPlanner_p_m;
}


void CProbot::ComputeNeighbours(
    std::vector<NearNode> &Neighbours, int NewNode )
{
    
    static std::vector<NearNode> NearestComponentNodes ;
    CPersonajeConfig c = Graph_p_m->NodeConfig( NewNode ) ;
    // Mag niet static!
    
    static NearNode CurNode ;
    double CurDist ;
    
    Neighbours.resize(0) ;
    
    if ( !Parametros_m.TryAll )
    {
        NearestComponentNodes.resize( Graph_p_m->ComponentNumb ) ;
        
        for ( int j=0 ; j<(int)(NearestComponentNodes.size()) ; j++ )
            NearestComponentNodes[j].d = Infin ;
    } ;
    
    for( int i=0 ; i < Graph_p_m->NodeNumber()-1 ; i++ )
    {
        CurDist = LocalPlanner_p_m->SquareDistance(c, Graph_p_m->NodeConfig( i ) ) ;
        if ( CurDist <=
            ( Parametros_m.NeighbourDistance * Parametros_m.NeighbourDistance )
        )
        {
            if(  Parametros_m.TryAll )
            { // Strong Forest
            CurNode = NearNode( i, CurDist ) ;
            Neighbours.push_back( CurNode ) ;
        }
        else
            if ( CurDist <
                NearestComponentNodes[Graph_p_m->NodeComponent(i)].d )
            {
                NearestComponentNodes[Graph_p_m->NodeComponent(i)].i = i ;
                NearestComponentNodes[Graph_p_m->NodeComponent(i)].d = CurDist ;
            }
    }
} ;

if(  !Parametros_m.TryAll )
    for ( int k=0 ; k<(int)(NearestComponentNodes.size()) ; k++ )
        if ( NearestComponentNodes[k].d != Infin )
            Neighbours.push_back( NearestComponentNodes[k] ) ;
        
        if(  !Parametros_m.AllowLoops && Parametros_m.TryAll )
            // Sort with Strong Forest
        sort( Neighbours.begin(), Neighbours.end(), DistanceCompare ) ;
    }
    
    
    
    /*** memberfunction ***
     * name :     AddEdges
     * class :    uu_MotionPlanner
     * input :    int NewNode, const std::vector<NearNode> &Neighbours
     * output :   void
     * author :   Valerie Boor
     * date :     08/30/1999
     * see also : BuildRoadmap, ComputeNeighbours, AddNode
     * description :
     *   private function only to be used by uu_MotionPlanner::BuildRoadmap()
     *** end of memberfunction ***/
    void CProbot::AddEdges(
        int NewNode, const std::vector<NearNode> &Neighbours )
    {
        int j ;
        int ConnectNumber = 0 ;
        double PathLength ;
        
        for( j = 0; j < (int)(Neighbours.size()); j++ )
        {
            int idx = Neighbours[j].Index();
            
            if ( !Parametros_m.AllowLoops
                &&
                ( Graph_p_m->NodeComponent( NewNode ) == Graph_p_m->NodeComponent( idx ) )
            )
                continue ;
                
                if ( LocalPlanner_p_m->CheckPath(
                    Graph_p_m->NodeConfig( NewNode ),
                                                 Graph_p_m->NodeConfig( idx ),
                                                 PathLength ) )
                {
                    Graph_p_m->AddEdge( NewNode, idx, PathLength ) ;
                    ConnectNumber++ ;
                    if ( ConnectNumber >= Parametros_m.NeighbourNumber ) break ;
                }
        }
    }
    
    
    
    const uu_PlanningParameters & CProbot::Parametros() const
    {
        return Parametros_m ;
    }
    
    const CAmbiente * CProbot::Ambiente()const
    {
        return (Ambiente_p_m);
    }
    
    
    const CSamples * CProbot::Samples() const
    {
        return Samples_p_m;
    }
    
    
    const std::vector<uu_Point> & CProbot::Metas()const
    {
        return listametas;
    }
    
    
    //-----------------------
    
    int CProbot::ComputeRecorridoC(const CPersonajeConfig &ini, const CPersonajeConfig &fin)
    {
        int                    j, idx ;
        bool                   Result_b_v = false ;
        double                 PathLength ;
        std::vector<NearNode>  Neighbours ;
        static NearNode        CurNode ;
        double                 CurDist ;
        std::vector < CPersonajeConfig > Steps_v ;
        std::vector < CPersonajeConfig > TempSteps_v ;
        
        uu_Graph * Graph_p_v = (uu_Graph *)Graph() ;
        
        // First connect the start
        Neighbours.resize(0) ;
        for( int i=0 ; i < (int)(Graph_p_m->NodeNumber()-1) ; i++ )
        {
            CurDist = LocalPlanner_p_m->SquareDistance( ini , Graph_p_m->NodeConfig( i ) ) ;
            CurNode = NearNode( i, CurDist ) ;
            Neighbours.push_back( CurNode ) ;
        }
        std::sort( Neighbours.begin(), Neighbours.end(), DistanceCompare ) ;
        
        for( j = 0; ( (j < (int)(Neighbours.size())) && (!Result_b_v) );  j++ )
        {
            idx = Neighbours[j].Index();
            if ( LocalPlanner_p_m->CheckPath(
                ini,
                Graph_p_m->NodeConfig( idx ),
                                             PathLength ) )
            {
                Result_b_v = true ;
                Graph_p_v->SetStartNode(idx) ;
                LocalPlanner_p_m->ReturnPath( ini ,
                                              Graph_p_v->NodeConfig( idx ) ) ;
                                              for (int i=0;i<(int)( LocalPlanner_p_m->LocalPath().size());i++)
                                                  Steps_v.push_back( LocalPlanner_p_m->LocalPath()[i]) ;
            }
        }
        
        if (Result_b_v)
        {
            Result_b_v = false ;
            // Second connect the goal
            Neighbours.resize(0) ;
            for( int i=0 ; i < Graph_p_m->NodeNumber()-1 ; i++ )
            {
                CurDist = LocalPlanner_p_m->SquareDistance(fin , Graph_p_m->NodeConfig( i ) ) ;
                CurNode = NearNode( i, CurDist ) ;
                Neighbours.push_back( CurNode ) ;
            }
            std::sort( Neighbours.begin(), Neighbours.end(), DistanceCompare ) ;
            for( j = 0; ( (j < (int)(Neighbours.size())) && (!Result_b_v) );  j++ )
            {
                idx = Neighbours[j].Index();
                if ( LocalPlanner_p_m->CheckPath( fin , Graph_p_m->NodeConfig( idx ), PathLength ) )
                {
                    Result_b_v = true ;
                    Graph_p_v->SetGoalNode(idx) ;
                    if (Graph_p_v->Connected( Graph_p_v->StartNode(), Graph_p_v->GoalNode()) )
                    {
                        ComputePath() ;
                        
                        for (int i=0;i<(int)(recorrido.size());i++)
                            Steps_v.push_back(recorrido[i]) ;
                        LocalPlanner_p_m->ReturnPath( Graph_p_m->NodeConfig(idx), fin );
                        for (int i=0;i<(int)( LocalPlanner_p_m->LocalPath().size());i++)
                            Steps_v.push_back( LocalPlanner_p_m->LocalPath()[i]) ;
                        
                        
                        recorrido.clear();
                        listametas.clear();
                        recorrido = Steps_v;
                        Smooth();
                        Calcularmetas();
                        
                    }
                    else
                    {
                        return 0 ;
                    }
                    
                }
            }
        }
        
        if (Result_b_v)
            return 1 ;
        
        else
            return 0 ;
        
    }
    
    
    
    int CProbot::ComputeRecorridoLF(const CPersonajeConfig &Inicio_p_a)
    {
        int                    j, idx ;
        bool                   Result_b_v = false ;
        double                 PathLength ;
        std::vector<NearNode>  Neighbours ;
        static NearNode        CurNode ;
        double                 CurDist ;
        std::vector < CPersonajeConfig > Steps_v ;
        
        
        // First connect the start
        Neighbours.resize(0) ;
        for( int i=0 ; i < Graph_p_m->NodeNumber()-1 ; i++ )
        {
            CurDist = LocalPlanner_p_m->SquareDistance( Inicio_p_a , Graph_p_m->NodeConfig( i ) ) ;
            CurNode = NearNode( i, CurDist ) ;
            Neighbours.push_back( CurNode ) ;
        }
        
        std::sort( Neighbours.begin(), Neighbours.end(), DistanceCompare ) ;
        
        for( j = (int)(Neighbours.size()-1); (( j >=0 ) && (!Result_b_v) );  j-- )
        {
            idx = Neighbours[j].Index();
            
            if ( LocalPlanner_p_m->CheckPath( Inicio_p_a, Graph_p_m->NodeConfig( idx ), PathLength ) )
            {
                Result_b_v = true ;
                Graph_p_m->SetStartNode(idx) ;
                break;
            }
        }
        
        
        if (Result_b_v)
        {
            recorrido.clear();
            listametas.clear();
            Graph_p_m->RecorridoprofundidadPC(idx, idx, idx,  &listametas);
            //if (listametas.size()<20) return 0;  //pocos puntos
            
            /* for ( int l = 0; l < (int)(listametas.size() - 1) ; l++)
             *      {
             *          CPersonajeConfig p1, p2;
             *          p1.SetPosition(listametas[l]);
             *          p2.SetPosition(listametas[l+1]);
             * 
             *          LocalPlanner_p_m->ReturnPath( p1, p2 ) ;
             *          for (int h=0; h<(int)( LocalPlanner_p_m->LocalPath().size());h++)
             *           recorrido.push_back( LocalPlanner_p_m->LocalPath()[h]) ;
             * 
             * //        cout<<l<<": "<<listametas[l].X()<<"  "<<listametas[l].Y()<<"  "<<listametas[l].Z() << endl;
             }
             */
            return 1 ;
        }
        
        return 0 ;
    }
    
    
    
    int CProbot::ComputePath()
    {
        int i, j ;
        std::vector<int> Nodes_v;
        std::vector< CPersonajeConfig > LocalPath_v ;
        
        
        if ( (Graph_p_m != NULL) && ( LocalPlanner_p_m != NULL))
        {
            Nodes_v.clear();
            recorrido.clear();
            Graph_p_m->ShortestPath( Graph_p_m->StartNode(), Graph_p_m->GoalNode() , Nodes_v ) ;
            for (i = 0; i < (int)(Nodes_v.size())-1 ; i++ )
            {
                if( LocalPlanner_p_m->ReturnPath( Graph_p_m->NodeConfig( Nodes_v[i]),
                    Graph_p_m->NodeConfig( Nodes_v[i+1] ) )
                )
                    for ( j=0 ; j<(int)(LocalPlanner_p_m->LocalPath().size()) ; j++ )
                        recorrido.push_back( LocalPlanner_p_m->LocalPath()[j] ) ;
                    else if ( LocalPlanner_p_m->ReturnPath( Graph_p_m->NodeConfig( Nodes_v[i+1] ),
                        Graph_p_m->NodeConfig( Nodes_v[i] ) )
                    )
                        for ( j=(int)(LocalPlanner_p_m->LocalPath().size())-1 ; j>=0 ; j-- )
                            recorrido.push_back( LocalPlanner_p_m->LocalPath()[j] ) ;
                        else
                            cout<< "PRM: ERROR : Trayectoria principal no puede ser reconstruida."<<endl ;
            }
            recorrido.push_back( Graph_p_m->NodeConfig( Nodes_v[ Nodes_v.size()-1 ] )) ;
            
        }
        
        else
        {
            return 0;
        }
        
        
        return 1 ;
    }
    
    
    void CProbot::Smooth()
    {
        unsigned int t2 ;
        unsigned int t3 ;
        clock_t StartingTime = 0, SmoothingTime = 0 ;
        
        if ( LocalPlanner_p_m != NULL )
            switch ( Parametros_m.SmoothingMethod )
            {
                case PRM::LP_SMOOTHING :
                {
                    SmoothingTime = Parametros_m.SmoothingTime ;
                    StartingTime = clock() ;
                    
                    while ( ((clock()-StartingTime) < SmoothingTime) && (!( Parametros_m.Interrupt)) )
                    {
                        int i = rand() % (recorrido.size()-1);
                        int j = rand() % (recorrido.size()-1);
                        
                        if ( i==j ) continue ;
                        
                        if ( j<i ) { int Temp = i ; i=j ; j=Temp ; }
                        
                        // choosing start and end of path more often
                        if ( (rand()%10) == 5 ) i = 0 ;
                        if ( (rand()%10) == 5 ) j = recorrido.size()-1 ;
                        
                        
                        if ( !(LocalPlanner_p_m->ReturnPath(
                            recorrido[i], recorrido[j])) ) continue ;
                        
                        if ( (int)(LocalPlanner_p_m->LocalPath().size()) < j-i )
                        {
                            for ( t2=0 ; t2<LocalPlanner_p_m->LocalPath().size() ; t2++ )
                                recorrido[i+t2] = LocalPlanner_p_m->LocalPath()[t2] ;
                            
                            int LengthDiff = (j-i) - LocalPlanner_p_m->LocalPath().size()+1 ;
                            for ( t3=i+LocalPlanner_p_m->LocalPath().size() ; t3< recorrido.size()-LengthDiff ; t3++ )
                                recorrido[t3] = recorrido[t3+LengthDiff] ;
                            
                            recorrido.resize( recorrido.size()-LengthDiff ) ;
                        }
                        
                    }
                    return ;
                }
                
            }
            
    }
    
    void CProbot::Calcularmetas()
    {
        double angle1,angle2;
        uu_Point p1,p2;
        
        angle1 = angle2 = 0.0;
        listametas.clear();
        if (recorrido.size()>0)
        {
            for (int i=0; i<(int)(recorrido.size()-1); i++)
            {
                p1 = recorrido[i].Position();
                p2 = recorrido[i+1].Position();
                angle1 = atan2( -(p2.Y() - p1.Y()), p2.X() - p1.Y());
                //              if ( fabs(angle1 - angle2) > 0.013962) // .8 grados aprox.  //HUSSEIN
                //              {                                                           //HUSSEIN
                listametas.push_back(p1);
                cout<<i<<":listametas: "<<p1.X()<<"  "<<p1.Y()<<
                "  "<<angle1<<"  "<<angle2<<endl;
                //              }                                                           //HUSSEIN
                angle2 = angle1;
            }
            listametas.push_back(p2);
        }
        
        
    }
    
    //LA ÚNICA QUE OCUPAMOS (HUSSEIN)
    int CProbot::ProcesarChelas( int row1, int col1, double ori1,int row2, int col2, double ori2)
    {
        unsigned int i ;
        CPersonajeConfig robot1_v, robot2_v;
        
        uu_Point posicion;
        
        posicion.X() = (double)col1;
        posicion.Y() = (double)row1;
        posicion.Z() = ori1;
        
        robot1_v.SetPosition(posicion) ;
        
        posicion.X() = (double)col2;
        posicion.Y() = (double)row2;
        posicion.Z() = ori2;
        
        robot2_v.SetPosition(posicion) ;
        
        Limpiar();
        
        for (i = 0; i < 10; i++)        //máximo 10 intentos, incremeta 10 nodos nuevos por intentos
    {
        BuildRoadMap();
        if ( !ComputeRecorridoC(robot1_v, robot2_v) )
            cout<<"PRM: intento: "<<i<<" No hay trayectoria"<<endl;
        else
        {
            cout<<i<<"PRM: Trayectoria encontrada de (x: "<<col1<<" , y: "<<row1<<
            " ) a (x: "<<col2<<" , y: "<<row2<<" )"<<endl;
            break;
        }
        Parametros_m.NodeNumber += 100;
    }
    if (i == 10)
        return 0;   //no se generaron bien los puntos
        
        return 1;
    }
    
    
    int CProbot::ProcesarLostF( int row, int col, double ori)
    {
        unsigned int i ;
        CPersonajeConfig robot_v;
        uu_Point posicion;
        
        posicion.X() = (double)col;
        posicion.Y() = (double)row;
        posicion.Z() = ori;
        
        robot_v.SetPosition(posicion) ;
        
        Limpiar();
        for (i = 0; i < 10; i++)        //máximo 10 intentos para generar las metas, se incrementa
    {                               // 10 nodos al grafo
    BuildRoadMap();
    if ( !ComputeRecorridoLF(robot_v) )
        cout<<"PRM: intento: "<<i<<" no se pudo conectar posicion del robot en el grafo o hay menos de 20 metas"<<endl;
    else
    {
        cout<<"PRM: Estrategía de búsqueda terminada, con un total de metas: "<<listametas.size()<<endl;
        break;
    }
    Parametros_m.NodeNumber += 10;
    }
    if (i == 10)
        return 0;   //no se generaron bien los puntos
        
        return 1;
    }
    
    
    
    
    int CProbot::ProcesarLostFFile( int row, int col, double ori)
    {
        CPersonajeConfig robot_v;
        uu_Point posicion;
        string file = "grafo";
        
        posicion.X() = (double)col;
        posicion.Y() = (double)row;
        posicion.Z() = ori;
        
        robot_v.SetPosition(posicion) ;
        
        if (Graph_p_m != NULL)
        {
            Graph_p_m->Clear();
            Graph_p_m->Read(file);
            SetLocalPlanner( new TranslateOnlyPlanner(this) ) ;
            if ( LocalPlanner_p_m == NULL)
            {
                cout<<"No local planner"<<endl ;
                return 1;
            }
            if ( !ComputeRecorridoLF(robot_v) )
                cout<<"PRM: No se pudo conectar posicion del robot en el grafo"<<endl;
            else
                cout<<"PRM: Estrategía de búsqueda terminada, con un total de metas: "<<listametas.size()<<endl;
            
        }
        else
            return 1;
        return 0;
    }
    
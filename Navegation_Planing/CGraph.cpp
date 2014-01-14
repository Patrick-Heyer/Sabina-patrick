#include "CGraph.h"
#include "CLocal.h"
#include "CProbot.h"
#include "uu_Utils.h"
//------------------------------------------------------------

//////////////////////////////////////////////////////////////
////////////////// Methods of class uu_Queue /////////////////
//////////////////////////////////////////////////////////////

uu_Queue::uu_Queue( const int Size )
{
    QHead = QTail = 0 ;
    QSize = Size ;
    TheQueue.clear() ;
    for ( int i = 0 ; i<Size ; i++ ) TheQueue.push_back(0) ;
}


int uu_Queue::IsEmpty () const
{
    return QHead == QTail ;
}


void uu_Queue::Enqueue( const int i )
{
    if ( Size() == QSize-1 )
    {
        uu_Utils::FlushConsole("ERROR: Error in enqueue -- queue overflow.\n" ) ;
    }
    else
    {
        TheQueue[QTail] = i ;
        QTail = ( QTail+1 ) % QSize ;
    }
}


int uu_Queue::Dequeue ()
{
    int Hold ;
    if ( IsEmpty() )
    {
        uu_Utils::FlushConsole("ERROR: Error in dequeue -- queue is empty.\n" ) ;
        Hold =  MAXINT ;
    }
    else
    {
        Hold = TheQueue[QHead] ;
        QHead = (QHead+1) % QSize ;
    }
    return Hold ;
}


void uu_Queue::MakeEmpty ()
{
    QHead = QTail = 0 ;
}


void uu_Queue::Print()
{
    std::string Error_s_v = "ERROR: " ;
    int i = QHead ;
    while ( i != QTail )
    {
        Error_s_v = "ERROR: " ;
        Error_s_v += "\n" ;
        uu_Utils::FlushConsole(Error_s_v.c_str()) ;
        i = (i+1) % QSize ;
    }
}


int uu_Queue::Size()
{
    int s = QTail - QHead ;
    if ( s < 0 ) s += QSize ;
    return s ;
}


//////////////////////////////////////////////////////////////
////////////////// Methods of struct uu_Node //////////////////
//////////////////////////////////////////////////////////////


uu_Node::uu_Node()
{
    Edges.clear() ;
    Lengths.clear() ;
    visited = false;
}

uu_Node::~uu_Node()
{
    Edges.clear() ;
    Lengths.clear() ;
}


/*** memberfunction ***
name :     Print
class :    uu_Node
input :    void
output :   void
author :   Valerie Boor
date :     08/31/1999
see also : uu_RobotConfig::Print, uu_Graph::Print
description :
  P.Print() configuration and edges are appended to the Error Output.
*** end of memberfunction ***/
void uu_Node::Print()
{
    unsigned int i ;
    uu_Utils::FlushConsole("Configuration: \n" /*<< Config*/) ;
    uu_Utils::FlushConsole("Edges: ") ;
    for ( i=0 ; i<Edges.size() ; i++ )
    {
        uu_Utils::FlushConsole(" ") ;
    }
    uu_Utils::FlushConsole("\n") ;
}


//////////////////////////////////////////////////////////////
////////// Local data & functions of class uu_Graph //////////
//////////////////////////////////////////////////////////////
struct ding
{
    int PathFrom, Visited ;
    double Length ;
} ;


std::vector<ding> TheCells ;


int uu_Graph::EnqueueCell ( uu_Queue &Q, int Cell, int Nbor, int Goal )
{
    if ( TheCells[Cell].Length > TheCells[Goal].Length )
        return false;//FALSE ;

    double Cost = EdgeLength( Cell, Nbor ) ;

    if ( ( TheCells[Nbor].Visited == 0
            ||
            TheCells[Cell].Length+Cost < TheCells[Nbor].Length) )
    {
        TheCells[Nbor].PathFrom = Cell ;
        TheCells[Nbor].Length = TheCells[Cell].Length+Cost ;
        TheCells[Nbor].Visited = 1 ;
        Q.Enqueue( Nbor ) ;
        return true ;
    }
    return false ;
}


int uu_Graph::EnqueueCell ( uu_Queue &Q, int Cell, int Nbor )
{
    double Cost = EdgeLength( Cell, Nbor ) ;

    if ( (TheCells[Nbor].Visited == 0
            ||
            TheCells[Cell].Length+Cost < TheCells[Nbor].Length) )
    {
        TheCells[Nbor].PathFrom = Cell ;
        TheCells[Nbor].Length = TheCells[Cell].Length+Cost ;
        TheCells[Nbor].Visited = 1 ;
        Q.Enqueue(Nbor) ;
        return true;//TRUE ;
    }
    return false; //FALSE ;
}


void uu_Graph::QueueFindPath ( int Source, int Goal )
{
    int i, j ;
    uu_Queue Q( 100000 ) ; //XXX Gevaarlijke constante...
    // Waarom niet MAXINT? te groot?
    std::vector<int> Neighbours ;

    TheCells[Source].PathFrom = -1 ;
    TheCells[Source].Length = 0 ;
    TheCells[Source].Visited = 1 ;
    Q.Enqueue( Source ) ;

    while ( !Q.IsEmpty() )
    {
        int Head = Q.Dequeue() ;

        /* Try all neighbors of the cell */
        Neighbours.clear() ;

        for ( j=0 ; j<(int)(Nodes[Head].Edges.size()) ; j++ )
            Neighbours.push_back( Nodes[Head].Edges[j] ) ;

        for ( i=0 ; i<(int)(Neighbours.size()) ; i++ )
            EnqueueCell( Q, Head, Neighbours[i], Goal ) ;
    }
}


void uu_Graph::QueueFindPath ( int Source )
{
    int i, j ;
    uu_Queue Q( 100000 ) ; //XXX Gevaarlijke constante...
    // Waarom niet MAXINT? te groot?
    std::vector<int> Neighbors ;

    TheCells[Source].PathFrom = -1 ;
    TheCells[Source].Length = 0 ;
    TheCells[Source].Visited = 1 ;
    Q.Enqueue(Source) ;

    while ( !Q.IsEmpty() )
    {
        int Head = Q.Dequeue() ;

        /* Try all neighbors of the cell */
        Neighbors.clear() ;
        for ( j=0 ; j<(int)(Nodes[Head].Edges.size()) ; j++ )
            Neighbors.push_back(Nodes[Head].Edges[j]) ;

        for ( i=0 ; i<(int)(Neighbors.size()) ; i++ )
            EnqueueCell( Q, Head, Neighbors[i] ) ;
    }
}


//////////////////////////////////////////////////////////////
////////////////// Methods of class uu_Graph /////////////////
//////////////////////////////////////////////////////////////



/*** memberfunction ***
name :     ComputeShortestGraphPath
class :    uu_Graph
input :    int, int, std::vector<int> &
output :   int
author :   Valerie Boor
date :     08/31/1999
see also : ShortestPath
description :
  P.ComputeShortestGraphPath(n2, n1, list) computes the shortest path
  from n1 to n2 in the graph and puts it in list. Returns 0 upon failure,
  1 when successful.
*** end of memberfunction ***/
int uu_Graph::ComputeShortestGraphPath( int n2, int n1, std::vector<int> &List )
{
    int j ;
    ding h ;

    if ( n1 < 0 || n2 < 0 ) return false;//FALSE ;

    TheCells.clear() ;

    for ( j=0 ; j<(int)(__Probot_p_m->Graph()->Nodes.size()) ; j++ )
    {
        h.Visited = 0 ;
        h.Length = MAXDOUBLE ;
        h.PathFrom = -1 ;
        TheCells.push_back(h) ;
    }

    QueueFindPath( n1, n2 ) ;

    int n = n2 ;
    while ( TheCells[n].PathFrom >= 0 )
    {
        List.push_back( n ) ;
        n = TheCells[n].PathFrom ;
    }
    List.push_back( n1 ) ;

    return ( TheCells[n2].Visited == 1 ) ;
}


/*** memberfunction ***
name :     uu_Graph
class :    uu_Graph
input :    uu_MotionPlanner *MP_p_a
output :   void
author :   Valerie Boor
date :     08/31/1999
see also :
description :
  Standard Constructor of class uu_Graph_p_m-> Sets EdgeNumb and ComponentNumb
  to 0, resizes Nodes to 2. (The Start and Goal node)
*** end of memberfunction ***/
uu_Graph::uu_Graph( CProbot *Probot_p_a )
{
    __Probot_p_m = Probot_p_a ;
    EdgeNumb = 0 ;
    ComponentNumb = 0 ;
    Nodes.resize(0) ;
    __Start_i_m = -1 ;
    __Goal_i_m  = -1 ;  // index of start and goal nodes (-1 if not specified)
}

/*** memberfunction ***
name :     NodeNumber
class :    uu_Graph
input :    void
output :   int
author :   Valerie Boor
date :     08/31/1999
see also :
description :
  G.NodeNumber() returns the number of nodes in the Graph G.
*** end of memberfunction ***/
int uu_Graph::NodeNumber() const
{
    return Nodes.size() ;
}


/*** memberfunction ***
name :     EdgeNumber
class :    uu_Graph
input :    void
output :   int
author :   Valerie Boor
date :     08/31/1999
see also :
description :
  G.EdgeNumber() returns the number of edges in the Graph G.
*** end of memberfunction ***/
int uu_Graph::EdgeNumber() const
{
    return EdgeNumb ;
}


/*** memberfunction ***
name :     ComponentNumber
class :    uu_Graph
input :    void
output :   int
author :   Valerie Boor
date :     08/31/1999
see also :
description :
  G.ComponentNumber() returns the number of components in the Graph G.
*** end of memberfunction ***/
int uu_Graph::ComponentNumber() const
{
    return ComponentNumb ;
}


/*** memberfunction ***
name :     AddNode
class :    uu_Graph
input :    CPersonajeConfig
output :   int
author :   Valerie Boor
date :     08/31/1999
see also : AddNodeAt, AddEdge
description :
  G.AddNode( q ) adds a node with given
  configuration to the graph G.
  Returns its index or -1 if no node could be added
*** end of memberfunction ***/
int uu_Graph::AddNode( CPersonajeConfig q )
{
    uu_Node h ;
    h.Config = q ;
    h.Edges.clear() ;
    h.Lengths.clear() ;
    h.Component = ComponentNumb ;
    Nodes.push_back(h) ;
    ComponentNumb++ ;

    /*hereee  _BEGINVISUALISATION_
          ActiveVisualisation_p_f->Graph()->AddNode(
          q.Position().X(), q.Position().Y(), q.Position().Z() ) ;
      _ENDVISUALISATION_
    */
    return Nodes.size()-1 ;
}


/*** memberfunction ***
name :     AddNodeAt
class :    uu_Graph
input :    uu_RobotConfig &, int
output :   int
author :   Valerie Boor
date :     08/31/1999
see also : AddNode, AddEdge
description :
  G.AddNodeAt(uu_RobotConfig & q, int n) adds a node with given
  configuration to the graph G. Is used in an otherwise empty graph to add
  start and goal to respectively G[0] and G[1]
*** end of memberfunction ***/
void uu_Graph::AddNodeAt( CPersonajeConfig q, int n )
{
    if ( (int)(Nodes.size()) > n )
    {
        uu_Node h ;
        h.Config = q ;
        h.Edges.clear() ;
        h.Lengths.clear() ;
        h.Component = n ;
        Nodes[n] = h ; // CAN ONLY BE USED FOR START & GOAL POSITIONS !!
    }
    else
        AddNode( q ) ;
}



/*** memberfunction ***
name :     AddEdge
class :    uu_Graph
input :    int, int, double
output :   int
author :   Valerie Boor
date :     08/31/1999
see also : AddNodeAt, AddNode
description :
  G.AddEdge( n1, n2, length ) adds an edge between the nodes
  with indices n1 and n2 with given path-length to the graph G.
  Returns 1 upon success, 0 upon failure.
*** end of memberfunction ***/
int uu_Graph::AddEdge( int n1, int n2, double PathLength )
{
    int i ;
    Nodes[n1].Edges.push_back( n2 ) ;
    Nodes[n1].Lengths.push_back( PathLength ) ;
    Nodes[n2].Edges.push_back( n1 ) ;
    Nodes[n2].Lengths.push_back( PathLength ) ;

    int Comp1 = Nodes[n1].Component ;
    int Comp2 = Nodes[n2].Component ;

    if ( Comp1 == Comp2 ) return 1 ;

    if ( Comp1 < Comp2 )
    {
        for ( i=0; i<(int)(Nodes.size()); i++ )
        {
            if ( Nodes[i].Component == Comp2 )
                Nodes[i].Component = Comp1 ;
            else if ( Nodes[i].Component == ComponentNumb-1 )
                Nodes[i].Component = Comp2 ;
        }
        ComponentNumb-- ;
    }
    else if ( Comp2 < Comp1 )
    {
        for ( i=0; i<(int)(Nodes.size()); i++ )
        {
            if ( Nodes[i].Component == Comp1 )
                Nodes[i].Component = Comp2 ;
            else if ( Nodes[i].Component == ComponentNumb-1 )
                Nodes[i].Component = Comp1 ;
        }
        ComponentNumb-- ;
    } ;

    EdgeNumb++ ;
    return 1 ;
}


/*** memberfunction ***
name :     EdgeLength
class :    uu_Graph
input :    int, int
output :   double
author :   Valerie Boor
date :     08/31/1999
see also : AddEdge
description :
  G.EdgeLength( n1, n2 ) returns the length of the path between nodes n1
  and n2, or 1000.0 if the path cannot be found.
*** end of memberfunction ***/
double uu_Graph::EdgeLength( int n1, int n2 )  const
{
    int k ;
    for ( k=0 ; k < (int)(Nodes[n1].Edges.size()) ; k++ )
        if ( Nodes[n1].Edges[k] == n2 )
            return Nodes[n1].Lengths[k] ;

    return 1000.0 ;
}


/*** memberfunction ***
name :     EdgesOfNode
class :    uu_Graph
input :    int
output :   std::vector<int>
author :   Valerie Boor
date :     08/31/1999
see also : AddEdge, EdgeLength
description :
  G.EdgesOfNode( n1 ) returns the list of indices corresponding to the
  edges of node n1.
*** end of memberfunction ***/
std::vector<int> uu_Graph::EdgesOfNode( int n ) const
{
    return Nodes[n].Edges;
}


/*** memberfunction ***
name :     DegreeOfNode
class :    uu_Graph
input :    int
output :   int
author :   Valerie Boor
date :     08/31/1999
see also : AddEdge, EdgeLength
description :
  G.DegreeOfNode( n1 ) returns the number of edges departing from node n1.
*** end of memberfunction ***/
int uu_Graph::DegreeOfNode( int n ) const
{
    return Nodes[n].Edges.size() ;
}


/*** memberfunction ***
name :     NodeConfig
class :    uu_Graph
input :    int
output :   uu_RobotConfig
author :   Valerie Boor
date :     08/31/1999
see also : AddEdge, EdgeLength
description :
  G.NodeConfig( n1 ) returns the configuration of the node with index n1.
*** end of memberfunction ***/
const CPersonajeConfig & uu_Graph::NodeConfig( int n ) const
{
    return Nodes[n].Config ;
}


/*** memberfunction ***
name :     NodeComponent
class :    uu_Graph
input :    int
output :   int
author :   Valerie Boor
date :     08/31/1999
see also : AddEdge, EdgeLength
description :
  G.NodeComponent( n1 ) returns the graph component number of the node
  with index n1.
*** end of memberfunction ***/
int uu_Graph::NodeComponent( int n ) const
{
    return Nodes[n].Component ;
}


/*** memberfunction ***
name :     Connected
class :    uu_Graph
input :    int, int
output :   int
author :   Valerie Boor
date :     08/31/1999
see also : AddEdge, EdgeLength, NodeConfig, NodeComponent
description :
  G.Connected( n1, n2 ) returns 1 if the nodes n1 and n2 are connected in
  the graph, i.e. when they have the same graph component number.
  Returns 0 otherwise.
*** end of memberfunction ***/
int uu_Graph::Connected( int n1, int n2 ) const
{
    return Nodes[n1].Component == Nodes[n2].Component ;
}



/*** memberfunction ***
name :     Clear
class :    uu_Graph
input :    void
output :   void
author :   Valerie Boor
date :     08/31/1999
see also : AddEdge, EdgeLength
description :
  G.Clear() clears the graph. [This function used to do this for all nodes
  except for the start and goal configurations. Only the positions of the
  start and goal node were kept. Now all nodes are removed from the graph,
  which is rather silly since we still use the fact that first and second
  node are start and goal node... Todo!!! Do something about it!!]
*** end of memberfunction ***/
void uu_Graph::Clear()
{
    Nodes.clear();
    EdgeNumb = 0 ;
    ComponentNumb = Nodes.size() ;
}


/*** memberfunction ***
name :     ShortestPath
class :    uu_Graph
input :    int, int, std::vector<int> &
output :   int
author :   Valerie Boor
date :     08/31/1999
see also : ComputeShortestGraphPath
description :
  P.ShortestPath(n2, n1, list) returns
  P.ComputeShortestGraphPath(n2, n1, list) which computes the shortest
  path from n1 to n2 in the graph and puts it in list. Returns 0 upon
  failure, 1 when successful.
*** end of memberfunction ***/
int uu_Graph::ShortestPath( const int &n2, const int &n1,
                            std::vector<int> &List )
{
    return ComputeShortestGraphPath( n2, n1, List ) ;
}


/*** memberfunction ***
name :     StartNode
class :    uu_Graph
input :    void
output :   int
author :   Valerie Boor
date :     09/11/1999
see also : GoalNode
description :
  Returns the index of the Start node
*** end of memberfunction ***/
int uu_Graph::StartNode() const
{
    return __Start_i_m ;
}

/*** memberfunction ***
name :     GoalNode
class :    uu_Graph
input :    void
output :   int
author :   Valerie Boor
date :     09/11/1999
see also : SetQuery, StartNode
description :
  Returns the index of the Goal node
*** end of memberfunction ***/
int uu_Graph::GoalNode() const
{
    return __Goal_i_m ;
}

/*** memberfunction ***
name :     StartNode
class :    uu_Graph
input :    void
output :   int
author :   Valerie Boor
date :     09/11/1999
see also : SetQuery, GoalNode
description :
  Returns the index of the Start node
*** end of memberfunction ***/
void uu_Graph::SetStartNode(int i_i_a)
{
    __Start_i_m = i_i_a;
}

/*** memberfunction ***
name :     GoalNode
class :    uu_Graph
input :    void
output :   int
author :   Valerie Boor
date :     09/11/1999
see also : SetQuery, StartNode
description :
  Returns the index of the Goal node
*** end of memberfunction ***/
void uu_Graph::SetGoalNode(int i_i_a)
{
    __Goal_i_m = i_i_a ;
}


/*** memberfunction ***
name :     ParseComponent
class :    uu_Graph
input :    int Component_i_a
output :   int
author :   Arno Kamphuis
date :     30/11/1999
see also :
description :
  Saves the graph from the file named Filename_s_a
*** end of memberfunction ***/
int uu_Graph::ParseComponent( int Component_i_a )
{
    int         RankingNumber_i_v ;
    int         Result_i_v ;
    std::string Name_s_v ;


    (*__Input_m) >> Name_s_v ;

    while ( (Name_s_v != "p3d_end_comp") && (Name_s_v != "\0") )
    {
        if ( Name_s_v != "p3d_node") return -2 ;
        (*__Input_m) >> RankingNumber_i_v ;

        Result_i_v = ParseNode( RankingNumber_i_v, Component_i_a ) ;
        if ( Result_i_v != 1 ) return Result_i_v ;

        (*__Input_m) >> Name_s_v ;
    }

    if (Name_s_v == "p3d_end_comp") ComponentNumb++ ;
    return 1 ;
}

/*** memberfunction ***
name :     ParseNode
class :    uu_Graph
input :    int RankingNumber_i_a, int Component_i_a
output :   int
author :   Arno Kamphuis
date :     30/11/1999
see also :
description :
  Saves the graph from the file named Filename_s_a
*** end of memberfunction ***/
int uu_Graph::ParseNode( int RankingNumber_i_a, int Component_i_a )
{
    int         Result_i_v ;
    int         NumberOfEdges_i_v ;
    int         i ;
    std::string Name_s_v ;
    std::string Value_s_v ;
    uu_Node     Node_v ;
    double      X_d_v, Y_d_v, Z_d_v ;
    uu_Point    Pos_v ;

    (*__Input_m) >> Name_s_v ;

    while ( Name_s_v != "p3d_end_node" )
    {
        Nodes[RankingNumber_i_a].Component = Component_i_a ;

        if ( Name_s_v != "p3d_q" ) return -3 ;
        (*__Input_m) >> X_d_v ;
        Pos_v.X() = X_d_v ;
        (*__Input_m) >> Y_d_v ;
        Pos_v.Y() = Y_d_v ;
        (*__Input_m) >> Z_d_v ;
        Pos_v.Z() = Z_d_v ;

        Nodes[RankingNumber_i_a].Config.SetPosition(Pos_v);

        (*__Input_m) >> Name_s_v ;
        if ( Name_s_v != "p3d_neighb" ) return -3 ;
        (*__Input_m) >> NumberOfEdges_i_v ;
        for (i=0; i<NumberOfEdges_i_v; i++)
        {
            (*__Input_m) >> Value_s_v ;
        }

        (*__Input_m) >> Name_s_v ;
        if ( Name_s_v != "p3d_nedge" ) return -3 ;
        (*__Input_m) >> NumberOfEdges_i_v ;

        for (i=0; i<NumberOfEdges_i_v; i++)
        {
            Result_i_v = ParseEdge(RankingNumber_i_a) ;
            if ( Result_i_v != 1 ) return Result_i_v ;
        }

        (*__Input_m) >> Name_s_v ;
    }
    return 1 ;
}

/*** memberfunction ***
name :     ParseEdge
class :    uu_Graph
input :    int RankingNumber_i_a
output :   int
author :   Arno Kamphuis
date :     30/11/1999
see also :
description :
  Saves the graph from the file named Filename_s_a
*** end of memberfunction ***/
int uu_Graph::ParseEdge( int RankingNumber_i_a )
{
    int         Id_i_v ;
    double      Length_d_v ;
    std::string Name_s_v ;
    std::string Value_s_v ;
    uu_Node     Node_v ;

    (*__Input_m) >> Name_s_v ;
    if ( Name_s_v != "p3d_edge") return -4 ;

    (*__Input_m) >> Id_i_v ;
    (*__Input_m) >> Id_i_v ;
    (*__Input_m) >> Value_s_v ;
    (*__Input_m) >> Length_d_v ;

    Nodes[RankingNumber_i_a].Edges.push_back(Id_i_v) ;
    Nodes[RankingNumber_i_a].Lengths.push_back(Length_d_v) ;

    EdgeNumb++ ;

    return 1 ;
}

/*** memberfunction ***
name :     Save
class :    uu_Graph
input :    std::string Filename_s_a
output :   void
author :   Arno Kamphuis
date :     30/11/1999
see also :
description :
  Saves the graph from the file named Filename_s_a
*** end of memberfunction ***/
void uu_Graph::Save( std::string Filename_s_a )
{
    int i, j, k, Id_i_v ;
    bool found ;
    std::vector < std::vector < int > > NodeIdPerComponent_v ;
    std::vector < int > Ids_v ;

    __Output_m = new ofstream( Filename_s_a.c_str() ) ;

    NodeIdPerComponent_v.resize( ComponentNumb ) ;

    for (i=0; i<(int)(Nodes.size()),(int)(Ids_v.size())!=ComponentNumb; i++)
    {
        found = false ;
        for (j=0; j<(int)(Ids_v.size()); j++)
            if (Ids_v[j] == Nodes[i].Component) found = true ;
        if (!found) Ids_v.push_back( Nodes[i].Component ) ;
    }

    for (i=0; i<(int)(Nodes.size()); i++)
        for (j=0; j<(int)(Ids_v.size()); j++)
            if (Ids_v[j] == Nodes[i].Component)
                NodeIdPerComponent_v[j].push_back( i ) ;

    (*__Output_m) << "p3d_env " << "...name..." << endl << endl ;
    (*__Output_m) << "p3d_nnodes " << Nodes.size() << endl << endl ;

    (*__Output_m) << "p3d_nbq " << -1 << endl ;
    (*__Output_m) << "p3d_nbq_free " << -1 << endl ;
    (*__Output_m) << "p3d_time " << -1.0 << endl ;
    (*__Output_m) << "p3d_local_call " << -1 << endl ;
    (*__Output_m) << "p3d_test_coll " << -1 << endl << endl ;

    for (i=0; i<ComponentNumb; i++)
    {
        (*__Output_m) << "p3d_comp " << i << endl << endl ;

        for (j=0; j<(int)(NodeIdPerComponent_v[Ids_v[i]].size()); j++)
        {
            Id_i_v = NodeIdPerComponent_v[Ids_v[i]][j] ;
            (*__Output_m) << "  p3d_node " << Id_i_v << endl ;

            (*__Output_m) << "    p3d_q "
                          << Nodes[Id_i_v].Config.Position().X() << " " <<
                          Nodes[Id_i_v].Config.Position().Y() << " " <<
                          Nodes[Id_i_v].Config.Position().Z() << " " << endl ;
            (*__Output_m) << "    p3d_neighb " << Nodes[Id_i_v].Edges.size() ;
            for (k=0; k<(int)(Nodes[Id_i_v].Edges.size()); k++)
                (*__Output_m) << " " << Nodes[Id_i_v].Edges[k] ;
            (*__Output_m) << endl ;
            (*__Output_m) << "    p3d_nedge " << Nodes[Id_i_v].Edges.size() << endl ;
            for (k=0; k<(int)(Nodes[Id_i_v].Edges.size()),k<(int)(Nodes[Id_i_v].Lengths.size()); k++)
            {
                (*__Output_m) << "    p3d_edge " << Id_i_v << " " << Nodes[Id_i_v].Edges[k]
                              << " " << "-1" << " " << Nodes[Id_i_v].Lengths[k] << endl ;
            }
            (*__Output_m) << "  p3d_end_node " << endl ;
        }
        (*__Output_m) << "  p3d_end_comp " << endl ;
    }

    __Output_m->close() ;
    delete __Output_m ;
    __Output_m = NULL ;
}

/*** memberfunction ***
name :     Read
class :    uu_Graph
input :    std::string Filename_s_a
output :   int
author :   Arno Kamphuis
date :     30/11/1999
see also :
description :
  Reads the graph from the file named Filename_s_a
*** end of memberfunction ***/
int uu_Graph::Read( std::string Filename_s_a )
{
    std::string Name_s_v ;
    std::string Value_s_v ;
    int         Component_i_v ;
    int         NumberOfNodes_i_v ;
    int         Result_i_v ;
    int         i ;

    __Input_m = new ifstream(Filename_s_a.c_str() ) ;

    Clear() ;

    (*__Input_m) >> Name_s_v ;
    (*__Input_m) >> Value_s_v ;

    (*__Input_m) >> Name_s_v ;
    if ( Name_s_v != "p3d_nnodes") return -1 ;
    (*__Input_m) >> NumberOfNodes_i_v ;

    Nodes.resize(NumberOfNodes_i_v) ;

    // input information that is not interesting
    for (i=0; i<5; i++)
    {
        (*__Input_m) >> Name_s_v ;
        (*__Input_m) >> Value_s_v ;
    }

    while (!(__Input_m->eof()))
    {
        (*__Input_m) >> Name_s_v ;
        if ( Name_s_v != "p3d_comp") return -1 ;
        (*__Input_m) >> Component_i_v ;

        Result_i_v = ParseComponent( Component_i_v ) ;
        if ( Result_i_v != 1 ) return Result_i_v ;
    }

    __Input_m->close() ;
    delete __Input_m ;

    return 1 ;
}


void uu_Graph::RecorridoprofundidadPC(int ind, int padre, int abuelo, std::vector<uu_Point> *list)
{
    int h;
    bool visitarotro = false;

    if (Nodes.size() == 0) return;

    list->push_back(Nodes[ind].Config.Position());
    Nodes[ind].visited = true;
//	std::cout<<ind<<std::endl;
    for (unsigned int i = 0; i < Nodes[ind].Edges.size(); i++)
    {
        //buscar el vecino más lejano
        if ( (Nodes[ind].Edges[i] != padre) && (Nodes[Nodes[ind].Edges[i]].visited != true) )
        {
            h = i ;
            for (int j = 0; j < (int)(Nodes[ind].Edges.size()) ; j++)
                if ( (Nodes[ind].Edges[j] != padre) && (Nodes[Nodes[ind].Edges[j]].visited != true) )
                {
                    if ( Nodes[ind].Lengths[h] < Nodes[ind].Lengths[j] )
                    {
                        h = j ;
                        i = 0 ;
                    }
                }
            RecorridoprofundidadPC(Nodes[ind].Edges[h], ind, padre ,list);
        }

    }

    for (unsigned int i = 0; i < Nodes[padre].Edges.size(); i++)
    {
        if ( !Nodes[Nodes[padre].Edges[i]].visited  )
        {
            visitarotro = true;
            break;
        }
    }
    if (visitarotro)
        list->push_back(Nodes[padre].Config.Position());
    else
    {
        for (unsigned int i = 0; i < Nodes[abuelo].Edges.size(); i++)
        {
            if ( Nodes[abuelo].Edges[i] == ind )
            {
                list->push_back(Nodes[abuelo].Config.Position());
                visitarotro = true;
                break;
            }

        }
        if (!visitarotro)
            list->push_back(Nodes[padre].Config.Position());

    }
}

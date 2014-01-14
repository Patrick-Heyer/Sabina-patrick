//--------------------------------------------------------------------
#ifndef CGraphH
#define CGraphH
//--------------------------------------------------------------------
#include <values.h>
#include <iostream>
#include <fstream>
//--------------------------------------------------------------------
#include "CPersonajeConfig.h"
#include "TiposBasicos.h"


using namespace std;
//--------------------------------------------------------------------

// forward declaration
class CProbot ;
//--------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////
// //////////////////////// Queue //////////////////////////////////////
////////////////////////////////////////////////////////////////////////

/*** class ***
name :     uu_Queue
derived :
author :   Valerie Boor
date :     08/30/1999
see also : uu_Graph, uu_Node
description :
*** end of class ***/
class uu_Queue
{
  protected:

  std::vector<int> TheQueue ;
  int QHead, QTail, QSize ;

  public:

  uu_Queue( const int Size ) ;
  int IsEmpty () const ;
  void Enqueue ( const int i ) ;
  int Dequeue () ;
  void MakeEmpty () ;
  void Print () ;
  int Size () ;
} ;


/*** struct ***
name :     uu_Node
derived :
author :   Valerie Boor
date :     08/30/1999
see also : uu_PPPData, uu_GlobalPath, uu_RobotConfig
description :
  A uu_Node consist of a configuration, connected component number, vectors
  of indices and the lengths of the paths of the incident edges, this
  is a struct, for lack of interesting methods and good constructors
*** end of struct ***/
class uu_Node
{
  public:
  bool visited;				  // Util para generación de trayectoria Markovito
  CPersonajeConfig Config ;   // Configuration of the node
  int Component ;		    // Label of its connected component
  std::vector<int> Edges ;        // Indices of the edges
  std::vector<double> Lengths ;	// Lengths of paths to neighbours

  uu_Node() ;
  ~uu_Node() ;

  void Print() ;
} ;


////////////////////////////////////////////////////////////////////////
//  uu_Graph
////////////////////////////////////////////////////////////////////////


/*** class ***
name :     uu_Graph
derived :
author :   Valerie Boor
date :     08/30/1999
see also : uu_PPPData, uu_GlobalPath, uu_ObjectConfig, LocalPlanner
description :
  Consists of the samples (or nodes) and the paths (represented by the edges)
  computed by the Local Planner between those samples.
*** end of class ***/
class uu_Graph
{
  private:

    CProbot         * __Probot_p_m ;
    int                __Start_i_m ;  // index of start nodes (-1 if not specified)
    int                __Goal_i_m ;   // index of goal  nodes (-1 if not specified)

    ifstream *         __Input_m ;
    ofstream *         __Output_m ;

    int  ComputeShortestGraphPath( int, int, std::vector<int> &List ) ;
    int  EnqueueCell ( uu_Queue &, int, int, int ) ;
    int  EnqueueCell ( uu_Queue &, int, int ) ;
    void QueueFindPath ( int, int ) ;
    void QueueFindPath ( int ) ;

    int  ParseComponent( int ) ;
    int  ParseNode( int, int ) ;
    int  ParseEdge( int ) ;

  public:

    std::vector<uu_Node> Nodes ;	// The nodes in the graph
    int EdgeNumb ;		    // Total number of edges
    int ComponentNumb ;		// Number of connected components

    uu_Graph( CProbot * ) ;

    int NodeNumber() const ;
    int EdgeNumber() const ;
    int ComponentNumber() const ;
    int AddNode( CPersonajeConfig ) ;	// Adds node with given configuration to graph..
                                                              // ...returns its index
    void                  AddNodeAt( CPersonajeConfig , int ) ;
    int                   AddEdge( int, int, double ) ;
    double                EdgeLength( int, int ) const ;
    std::vector<int>      EdgesOfNode( int ) const ;
    int                   DegreeOfNode( int ) const ;
    const CPersonajeConfig & NodeConfig( int ) const ;
    int                   NodeComponent( int ) const ;
    int                   Connected( int, int ) const ;
    void                  Clear() ;
    int                   ShortestPath( const int &n1,
                                        const int &n2,
                                        std::vector<int> &list ) ;
    
    int                   StartNode() const ;
    int                   GoalNode() const ;

    int                   Read( std::string ) ;
    void                  Save( std::string ) ;


    void                  SetStartNode(int) ;
    void                  SetGoalNode(int) ;
    void 		  RecorridoprofundidadPC(int ind, int padre, int abuelo, std::vector<uu_Point> *);

} ;

#endif

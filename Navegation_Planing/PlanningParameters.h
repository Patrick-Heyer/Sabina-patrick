//---------------------------------------------------------
#ifndef PlanningParametersH
#define PlanningParametersH
//---------------------------------------------------------
#include <time.h>
#include <string>
//---------------------------------------------------------

/*** struct ***
name :      uu_PlanningParameters
derived :
author :   Valerie Boor
date :     09/02/1999
see also :
description
  Parameters used for planning Purposes:
  int Sigma => used in resampling with gaussians;
  int NodeNumber => nr of nodes to generate;
  int RunTime=> mseconds to run;
  int SmoothingTime=> mseconds to smooth;
  clock_t BeginTime, EndTime, ElapsedTime => timing information;
  int Method => method used;
  int SamplingMethod => sampling method used;
  int SmoothingMethod => smoothing method used;
  int Local => local method used;
  int Connect => connect or don't connect?;
  int TryAll => try connecting to all nodes within neighbourdistance..
  .. or only the closest node of different components;
  int AllowLoops => Allow edges that introduce loops in the graph.;
  double StepSize => stepsize;
  double NeighbourDistance => distance of neighbours;
  double NeighbourNumber => max number of neighbours to ..
  .. try to connect to;
  double RobotSize => robotsize: distance of furthest vertex ..
  .. to robot-origin;

*** end of struct ***/
struct uu_PlanningParameters
{
    int                 SamplingMethod ;       // sampling method used
    double              Sigma ;                // used in resampling with gaussians

    int                 Method ;               // method used (nodes, times or connect)
    int                 NodeNumber ;           // nr of nodes to generate
    int                 RunTime;               // mseconds to run
    int                 Connect ;              // connect or don't connect?

    int                 SmoothingMethod ;      // smoothing method used
    int                 SmoothingTime;         // mseconds to smooth

    clock_t        BeginTime ;            // timing information
    clock_t        EndTime ;              // timing information
    clock_t        ElapsedTime ;          // timing information

    int                 Local ;                // local method used DEPENDS OF CONF.TYPE
    double              StepSize ;             // stepsize
    double              NeighbourDistance ;    // distance of neighbours
    double              MinNeighbourDistance ; // distance min of neighbours
    double              NeighbourNumber ;      // max number of neighbours to try to connect to
    int                 ConfigurationType ;    // configuration type DEPENDS OF LOCAL METHOD
    int                 TryAll ;               // try connecting to all nodes within neighbourdistance..
    // .. or only the closest node of different components

    int                 AllowLoops ;           // Allow edges that introduce loops in the graph.

    int                 CurrentQuery ;         // Current Query (uu_MotionPlanner::SetCurrentQuery)

    bool                DisplayAlgo ;          // display algo selects whether or not to draw everything
    bool                AddQueryToRoadmap ;    // during roadmap buildup add query to roadmap
    bool                ReportStatistics ;     // report the statistics

    int                 RandomSeed ;           // the seed that is used for taking random numbers

    int                 CollisionDetection ;   // This is the collision detection library that is used.

    bool                Interrupt ;            // This boolean is used to interrupt the action

    uu_PlanningParameters() ;

    void Save( std::string ) const ;
    void Load( std::string ) ;
} ;

#endif

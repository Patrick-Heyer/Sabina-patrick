#include <fstream>
#include <iostream>
#include "PlanningParameters.h"
#include "Constants.h"
//---------------------------------------------------------------

using namespace std;

uu_PlanningParameters::uu_PlanningParameters()
{
    SamplingMethod        = PRM::RANDOM_S ;       // sampling method used
    Sigma                 = 1.0 ;                 // used in resampling with gaussians

    Method                = PRM::RUN_NODES ;       // to be used!!
    NodeNumber            = 300 ;	                // nr of nodes to generate
    RunTime               = 1000 ;		// mseconds to run
    Connect               = 1 ;			// connect or don't connect?

    SmoothingMethod       = PRM::LP_SMOOTHING ;   // smoothing method used
    SmoothingTime         = 4000 ;	        // mseconds to smooth

    BeginTime             = 0 ;
    EndTime               = 0 ;
    ElapsedTime           = 0 ;

    Local                 = PRM::TRANSLATE_LP  ;	// local method used; change only when changing uu_motionPlanner::uu_MotionPlanner()
    StepSize              = 6 ;//0.02 ;		// stepsize
    NeighbourDistance     = 40 ;	                // distance of neighbours
    MinNeighbourDistance  = 20 ;	                // distance min of neighbours
    NeighbourNumber       = 30 ;		        // max number of neighbours to try to connect to
    ConfigurationType     = PRM::CAR_LIKE    ;    // configuration type (see Constants.h)
    TryAll                = PRM::TRY_ALL ;    // try connecting to all nodes

    AllowLoops            = 1 ;	                // Allow edges that introduce loops in the graph.

    CurrentQuery          = -1 ;                  // Current Query (uu_MotionPlanner::SetCurrentQuery)

    DisplayAlgo           = true ;                // displaying every step in the algorithm
    AddQueryToRoadmap     = false ;               // during roadmap buildup add query to roadmap
    ReportStatistics      = true ;                // report the statistics

    RandomSeed            = 0 ;
    CollisionDetection    = 0;

    Interrupt = false;
}

//---------------------------------------------------------------

void uu_PlanningParameters::Save( std::string Filename_s_a ) const
{
    std::ofstream output(Filename_s_a.c_str()) ;

    output << "SamplingMethod " << SamplingMethod << endl ;
    output << "Sigma " << Sigma << endl ;

    output << "Method " << Method << endl ;
    output << "NodeNumber " << NodeNumber << endl ;
    output << "RunTime " << RunTime << endl ;
    output << "Connect " << Connect << endl ;

    output << "SmoothingMethod " << SmoothingMethod << endl ;
    output << "SmoothingTime " << SmoothingTime << endl ;

    output << "BeginTime " << BeginTime << endl ;
    output << "EndTime " << EndTime << endl ;
    output << "ElapsedTime " << ElapsedTime << endl ;

    output << "Local " << Local << endl ;
    output << "StepSize " << StepSize << endl ;
    output << "NeighbourDistance " << NeighbourDistance << endl ;
    output << "NeighbourNumber " << NeighbourNumber << endl ;
    output << "ConfigurationType " << ConfigurationType << endl ;
    output << "TryAll " << TryAll << endl ;

    output << "AllowLoops " << AllowLoops << endl ;

    output << "CurrentQuery " << CurrentQuery  << endl ;

    output << "DisplayAlgo " << DisplayAlgo << endl ;
    output << "AddQueryToRoadmap " << AddQueryToRoadmap << endl ;
    output << "ReportStatistics " << ReportStatistics  << endl ;

    output << "RandomSeed " << RandomSeed << endl ;

    output << "CollisionDetection " << CollisionDetection << endl ;

    output.close() ;
}

//---------------------------------------------------------------

void uu_PlanningParameters::Load( std::string Filename_s_a )
{
    std::ifstream input(Filename_s_a.c_str()) ;
    std::string   tempstring ;

    input >> tempstring >> SamplingMethod ;
    input >> tempstring >> Sigma ;

    input >> tempstring >> Method ;
    input >> tempstring >> NodeNumber ;
    input >> tempstring >> RunTime ;
    input >> tempstring >> Connect ;

    input >> tempstring >> SmoothingMethod ;
    input >> tempstring >> SmoothingTime ;

    input >> tempstring >> BeginTime ;
    input >> tempstring >> EndTime ;
    input >> tempstring >> ElapsedTime ;

    input >> tempstring >> Local ;
    input >> tempstring >> StepSize ;
    input >> tempstring >> NeighbourDistance ;
    input >> tempstring >> NeighbourNumber ;
    input >> tempstring >> ConfigurationType ;
    input >> tempstring >> TryAll ;

    input >> tempstring >> AllowLoops ;

    input >> tempstring >> CurrentQuery  ;

    input >> tempstring >> DisplayAlgo ;
    input >> tempstring >> AddQueryToRoadmap ;
    input >> tempstring >> ReportStatistics  ;

    input >> tempstring >> RandomSeed ;

    input >> tempstring >> CollisionDetection ;

    input.close() ;
}

//---------------------------------------------------------------

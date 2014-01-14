//------------------------------------------------------------
#ifndef ClocalH
#define ClocalH
//------------------------------------------------------------
#include "CPersonajeConfig.h"
//------------------------------------------------------------
class CProbot ; // forward declaration
//------------------------------------------------------------

/*** class ***
name :	   uu_LocalPlanner
derived :
author :   Valerie Boor
date :     08/11/1999
see also : StraightLinePlanner, PotentialFieldPlanner
description :
  Local Coordinate frame planner consists of a.o.:
   - a vector of Articulated Planners for the local
     planners of the robot arm(s)  (TODO)
   - a Distance and Square Distance function
   - a Local Path (Last Local Path found by planner)
   - a method to check whether a path exists
   - a method to return a path
*** end of class ***/

class uu_LocalPlanner
{
  protected:

    CProbot *                       __Probot_p_m ;
    std::vector < CPersonajeConfig >           __LocalPath_m ;

  public:

    int Count, CCCount ;

    uu_LocalPlanner( const CProbot * ) ;

    double Distance( const CPersonajeConfig &, const CPersonajeConfig & ) ;
    double SquareDistance( const CPersonajeConfig &,
                           const CPersonajeConfig & ) ;
    virtual int CheckPath( const CPersonajeConfig &,
                           const CPersonajeConfig &,
                           double &PathLength ) = 0 ;
    virtual int ReturnPath( const CPersonajeConfig &,
                            const CPersonajeConfig & ) = 0 ;

    void Clear() ;

    const std::vector < CPersonajeConfig > & LocalPath() const ;
} ;

//-------------------------------------------------------------------------

/*** class ***
name :	   TranslateOnlyPlanner
derived :  uu_LocalPlanner
author :   Valerie Boor
date :     08/11/1999
see also : PotentialFieldPlanner, uu_LocalPlanner
description :
  TODO
*** end of class ***/

class TranslateOnlyPlanner : public uu_LocalPlanner
{

  public:

    TranslateOnlyPlanner( const CProbot * ) ;
    double Distance( const CPersonajeConfig &, const CPersonajeConfig & ) ;
    double SquareDistance( const CPersonajeConfig &,
                           const CPersonajeConfig & ) ;
    int CheckPath( const CPersonajeConfig &, const CPersonajeConfig &,
                      double &PathLength ) ;
    int ReturnPath( const CPersonajeConfig &,
                    const CPersonajeConfig & ) ;
} ;

//-------------------------------------------------------------------------

#endif

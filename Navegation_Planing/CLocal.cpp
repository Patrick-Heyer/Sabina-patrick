//-------------------------------------------------------------------------
#include<math.h>
//-------------------------------------------------------------------------
#include "CLocal.h"
#include "Constants.h"
#include "CProbot.h"


/*** memberfunction ***
name :     uu_LocalPlanner
class :    uu_LocalPlanner
input :    CAnimacion *MP_p_a
output :   void
author :   Valerie Boor
date :     29/11/1999
see also : uu_LocalPlanner
description :
  Constructor of uu_LocalPlanner.
*** end of memberfunction ***/
uu_LocalPlanner::uu_LocalPlanner( const CProbot * MP_p_a )
{
    __Probot_p_m = (CProbot *)MP_p_a ;
}

/*** memberfunction ***
name :     Clear
class :    uu_LocalPlanner
input :    const uu_RobotConfig &, const uu_RobotConfig &
output :   double
author :   Valerie Boor
date :     08/11/1999
see also : SquareDistance, uu_RobotConfig
description :
  This memberfunction returns the distance between two robot configurations.
*** end of memberfunction ***/
void uu_LocalPlanner::Clear( )
{
    __LocalPath_m.clear() ;
}

/*** memberfunction ***
name :     Distance
class :    uu_LocalPlanner
input :    const uu_RobotConfig &, const uu_RobotConfig &
output :   double
author :   Valerie Boor
date :     08/11/1999
see also : SquareDistance, uu_RobotConfig
description :
  This memberfunction returns the distance between two robot configurations.
*** end of memberfunction ***/
double uu_LocalPlanner::Distance( const CPersonajeConfig &c1, const CPersonajeConfig &c2 )
{
    uu_Point dv = c2.Position() - c1.Position() ;

//hereee  Quaternion dq = c2.Orientation() -  c1.Orientation() ;

    return dv.Length(); // + length( dq ) ;
}


/*** memberfunction ***
name :     SquareDistance
class :    uu_LocalPlanner
input :    const uu_RobotConfig &, const CPersonajeConfig &
output :   double
author :   Valerie Boor
date :     08/11/1999
see also : Distance, CPersonajeConfig
description :
  This memberfunction returns the square of the distance between two
  robot configurations.
*** end of memberfunction ***/
double uu_LocalPlanner::SquareDistance( const CPersonajeConfig & c1, const CPersonajeConfig & c2 )
{
    uu_Point dv = c2.Position() - c1.Position() ;
// Quaternion dq = c2.Orientation() -  c1.Orientation() ;

    return dv.SqLength(); // + length2( dq )  ;
}


const std::vector < CPersonajeConfig > & uu_LocalPlanner::LocalPath() const
{
    return __LocalPath_m ;
}

//-------------------------------------------------------------------------

/*** memberfunction ***
name :     TranslateOnlyPlanner
class :    TranslateOnlyPlanner
input :    CAnimacion * MP_p_a
output :
author :   Valerie Boor
date :     29/11/1999
see also : uu_LocalPlanner
description :
  Constructor of TranslateOnlyPlanner.
*** end of memberfunction ***/
TranslateOnlyPlanner::TranslateOnlyPlanner( const CProbot * MP_p_a )
    : uu_LocalPlanner( MP_p_a )
{
}

/*** memberfunction ***
name :     Distance
class :    TranslateOnlyPlanner
input :    const CPersonajeConfig &, const CPersonajeConfig &
output :   double
author :   Valerie Boor
date :     08/11/1999
see also : SquareDistance, CPersonajeConfig
description :
  This memberfunction returns the distance between two robot configurations.
*** end of memberfunction ***/
double TranslateOnlyPlanner::Distance( const CPersonajeConfig &c1, const CPersonajeConfig &c2 )
{
    uu_Point dv = c2.Position() - c1.Position() ;

    return dv.Length() ;
}


/*** memberfunction ***
name :     SquareDistance
class :    TranslateOnlyPlanner
input :    const CPersonajeConfig &, const CPersonajeConfig &
output :   double
author :   Valerie Boor
date :     08/11/1999
see also : Distance, CPersonajeConfig
description :
  This memberfunction returns the square of the distance between two
  robot configurations.
*** end of memberfunction ***/
double TranslateOnlyPlanner::SquareDistance( const CPersonajeConfig & c1, const CPersonajeConfig & c2 )
{
    uu_Point dv = c2.Position() - c1.Position() ;

    return dv.SqLength() ;
}

/*** memberfunction ***
name :     CheckPath
class :    TranslateOnlyPlanner
input :    const CPersonajeConfig &c1, const CPersonajeConfig &c2,
           double &PathLength
output :   int
author :   Valerie Boor
date :     08/11/1999
see also : Distance, SquareDistance, CPersonajeConfig, PRM,
           uu_PlanningParameters, uu_LocalPlanner::CheckPath,
           uu_LocalPlanner::ReturnPath, ReturnPath
description :
  This memberfunction calls the local planning method to find
  a path between the two configurations given. The third
  parameter will be replaced by the length of the path found.
  StraightSteps is a straight-line planner in C-space that takes
  steps in the direction of the goal. Stepsize is defined by
  the constant __MP_p_m.Parameters.stps.
  The robot should be grown by 0.5*stepsize.
*** end of memberfunction ***/
int TranslateOnlyPlanner::CheckPath(
    const CPersonajeConfig &c1, const CPersonajeConfig &c2,
    double &PathLength )
{
    CPersonajeConfig TheConfig ;
//  TheConfig.SetPersonaje( __Animacion_p_m->Ambiente()->Personaje() ) ;

    PathLength = Distance( c1, c2 ) ;

    Count++ ;

    if ( PathLength <= __Probot_p_m->Parametros().StepSize )
    {
        // A collision check should be performed here too.
        if ( (__Probot_p_m->Ambiente()->Colision( c1 ) )|| (__Probot_p_m->Ambiente()->Colision( c2 ) ))
            return 0 ;

        return 1 ;
    }

    int StepNumber = int( PathLength / __Probot_p_m->Parametros().StepSize ) ;

    double XStep = ( c2.Position().X() - c1.Position().X() ) / StepNumber ;
    double YStep = ( c2.Position().Y() - c1.Position().Y() ) / StepNumber ;
    double ZStep = ( c2.Position().Z() - c1.Position().Z() ) / StepNumber ;

    uu_Point StepVector ;
    StepVector.X() = XStep ;
    StepVector.Y() = YStep ;
    StepVector.Z() = ZStep ;

    uu_Point P( c1.Position() ) ;

    for ( int i=0 ; i<StepNumber ; i++ )
    {
        P = P + StepVector ;
        TheConfig.SetPosition(P) ;
        CCCount++ ;
#ifdef __BORLANDC__
        Application->ProcessMessages() ;
#endif
        if ( __Probot_p_m->Ambiente()->Colision( TheConfig ) )
            return 0 ;
    }
    return 1 ;
}

/*** memberfunction ***
name :     ReturnPath
class :    TranslateOnlyPlanner
input :    std::vector<CPersonajeConfig> &LocalPath,
           const CPersonajeConfig &c1, const CPersonajeConfig &c2
output :   int
author :   Valerie Boor
date :     08/11/1999
see also : Distance, SquareDistance, CPersonajeConfig, PRM,
           uu_PlanningParameters, uu_LocalPlanner::CheckPath,
           uu_LocalPlanner::ReturnPath
description :
  This memberfunction calls the local planning method to compute
  the path between the two configurations given. Should only be
  called when CheckPath( const CPersonajeConfig &,
  const CPersonajeConfig &, double & ) has terminated successfully
  (i.e. when a path could be found) for the pair of configurations
  considered. After successful execution, the first
  parameter will be replaced by a series of robot
  configurations that form the steps of the path.
*** end of memberfunction ***/
int TranslateOnlyPlanner::ReturnPath(
    const CPersonajeConfig &c1, const CPersonajeConfig &c2 )
{
    CPersonajeConfig TheConfig ;
    //Quaternion Orientacion_v ;

    __LocalPath_m.clear() ;
    __LocalPath_m.push_back( c1 ) ;

    if ( Distance(c1,c2) <= __Probot_p_m->Parametros().StepSize )
    {
        // A collision check should be performed here too.
        __LocalPath_m.push_back( c2 ) ;
        if ( (__Probot_p_m->Ambiente()->Colision( c1 ) )|| (__Probot_p_m->Ambiente()->Colision( c2 ) ))
            return 0 ;
        return 1 ;
    }

    int StepNumber = int( Distance(c1,c2) / __Probot_p_m->Parametros().StepSize ) ;

    double XStep = ( c2.Position().X() - c1.Position().X() ) / StepNumber ;
    double YStep = ( c2.Position().Y() - c1.Position().Y() ) / StepNumber ;
    double ZStep = ( c2.Position().Z() - c1.Position().Z() ) / StepNumber ;

    uu_Point StepVector ;
    StepVector.X() = XStep ;
    StepVector.Y() = YStep ;
    StepVector.Z() = ZStep ;

    uu_Point P( c1.Position() ) ;

    for ( int i=0 ; i<StepNumber ; i++ )
    {
        P = P + StepVector ;
        TheConfig.SetPosition(P) ;
#ifdef __BORLANDC__
        Application->ProcessMessages() ;
#endif
        if ( __Probot_p_m->Ambiente()->Colision( TheConfig ) ) return 0 ;

        __LocalPath_m.push_back( TheConfig ) ;
    }

    __LocalPath_m.push_back( c2 ) ;
    return 1 ;
}
//-------------------------------------------------------------------------

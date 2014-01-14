

#include "CPersonajeConfig.h"

#include "CProbot.h"


///////////////////////////////////////////////////////////
//////////////////// CPersonajeConfig ///////////////////////
///////////////////////////////////////////////////////////

CPersonajeConfig::CPersonajeConfig()
{
  __P_m.X() = 0 ; __P_m.Y() = 0 ; __P_m.Z() = 0 ;
  __JointValues_p_m = NULL ;
  __UseRadians_b_m = true ;

  __RobotPoint_m.P[0] = 0.0 ; __RobotPoint_m.P[1] = 0.0 ; __RobotPoint_m.P[2] = 0.0 ;
  __WSPoint_m.P[0] = 0.0 ; __WSPoint_m.P[1] = 0.0 ; __WSPoint_m.P[2] = 0.0 ;
}

CPersonajeConfig::CPersonajeConfig( const uu_Point &P )
{
  __P_m = P ;
  __JointValues_p_m = NULL ;
  __UseRadians_b_m = true ;
  __RobotPoint_m.P[0] = 0.0 ; __RobotPoint_m.P[1] = 0.0 ; __RobotPoint_m.P[2] = 0.0 ;
  __WSPoint_m.P[0] = __P_m.P[0] + __RobotPoint_m.P[0] ;
  __WSPoint_m.P[1] = __P_m.P[1] + __RobotPoint_m.P[1] ;
  __WSPoint_m.P[2] = __P_m.P[2] + __RobotPoint_m.P[2] ;
}

/*** memberfunction ***
name :     Position
class :    CPersonajeConfig
input :
output :   uu_Point &
author :   Valerie Boor
date :     21/12/1999
see also : Orientation
description

*** end of memberfunction ***/
const uu_Point & CPersonajeConfig::Position() const
{
  return __P_m ;
}

/*** memberfunction ***
name :     SetPosition
class :    CPersonajeConfig
input :    const uu_Point &
output :   void
author :   Arno Kamphuis
date :     12/01/2000
see also : SetOrientation
description

*** end of memberfunction ***/
void CPersonajeConfig::SetPosition( const uu_Point & Position_r_a )
{
  __P_m = Position_r_a ;
  __WSPoint_m.P[0] = Position_r_a.P[0] + __RobotPoint_m.P[0] ;
  __WSPoint_m.P[1] = Position_r_a.P[1] + __RobotPoint_m.P[1] ;
  __WSPoint_m.P[2] = Position_r_a.P[2] + __RobotPoint_m.P[2] ;
}

/*** memberfunction ***
name :     InWorkspace
class :    CPersonajeConfig
input :    uu_MotionPlanner * MotionPlanner
output :   int
author :   Valerie Boor
date :     09/02/1999
see also :
description

*** end of memberfunction ***/
int  CPersonajeConfig::InWorkspace( const CProbot * Probot_p_a )
{
  return Probot_p_a->Ambiente()->Workspace()->Inside( __P_m ) ;
}



void CPersonajeConfig::SetRadians(bool Value_b_a)
{
  __UseRadians_b_m = Value_b_a ;
}


void CPersonajeConfig::SetWSPoint( const uu_Point & Pos_a )
{
  __WSPoint_m = Pos_a ;
}

void CPersonajeConfig::SetRobotPoint( const uu_Point & Pos_a )
{
  __RobotPoint_m = Pos_a ;
}


const uu_Point CPersonajeConfig::WSPoint() const
{
  return __WSPoint_m ;
}

const uu_Point CPersonajeConfig::RobotPoint() const
{
  return __RobotPoint_m ;
}




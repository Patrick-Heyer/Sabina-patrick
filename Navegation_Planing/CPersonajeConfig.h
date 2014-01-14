
//---------------------------------------------------------
#ifndef PersonajeConfig_H
#define PersonajeConfig_H
//---------------------------------------------------------
#include <vector>

#include "TiposBasicos.h"

class CProbot;
///////////////////////////////////////////////////////////
//////////////////// uu_RobotConfig ///////////////////////
///////////////////////////////////////////////////////////

/*** class ***
name :     uu_RobotConfig
derived :
author :   Valerie Boor
date :     09/02/1999
see also :
description
   stripped d.d. 20/12/99 by Valerie
*** end of class ***/

class CPersonajeConfig
{
  private:

    uu_Point   __P_m ;
    std::vector< double* > * __JointValues_p_m ;
    bool                     __UseRadians_b_m ;

    uu_Point   __RobotPoint_m ;
    uu_Point   __WSPoint_m ;
  public:

    CPersonajeConfig() ;
    CPersonajeConfig( const uu_Point & ) ;
    ~CPersonajeConfig(){};
    const uu_Point & Position() const ;
    void SetPosition( const uu_Point & ) ;

    int InWorkspace( const CProbot *  ) ;

    void SetRadians(bool) ;

    void SetWSPoint( const uu_Point & ) ;
    void SetRobotPoint( const uu_Point & ) ;

    const uu_Point WSPoint() const ;
    const uu_Point RobotPoint() const ;
} ;

#endif

//---------------------------------------------------------------------------
#ifndef CSamplesH
#define CSamplesH
//---------------------------------------------------------------------------
#include <vector>
//---------------------------------------------------------------------------
#include "CPersonajeConfig.h"
//---------------------------------------------------------------------------

/*** class ***
name :	   uu_Samples
derived :
author :   Arno Kamphuis
date :     05/10/2000
see also : uu_MotionPlanner
description :
*** end of class ***/

class CSamples
{
protected:
    std::vector < std::vector < CPersonajeConfig > > _Samples_m ;
public:

    CSamples() ;

    const CPersonajeConfig & Sample( int, int ) const ;
    const std::vector < CPersonajeConfig > & Samples( int ) const ;

    void AddSample( int, const CPersonajeConfig & ) ;

    void Clear() ;

    int Size() const ;
    int Size(int) const ;
} ;

#endif

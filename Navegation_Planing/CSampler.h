//---------------------------------------------------------------------------
#ifndef CSamplerH
#define CSamplerH
//---------------------------------------------------------------------------
#include <vector>
//---------------------------------------------------------------------------
#include "CPersonajeConfig.h"
//---------------------------------------------------------------------------

// forward definition
class CProbot ;

/*** class ***
name :	   uu_Sampler
derived :
author :   Valerie Boor
date :     10/11/1999
see also : CAnimacion, uu_GaussianSampler,
           uu_UniformRandomSampler
description :
  Generates Samples and Keeps a list of them
*** end of class ***/
class uu_Sampler
{
  protected:
    CProbot * _Probot_p_m ;
  public:
    uu_Sampler( const CProbot * ) ;
    virtual int Generate(CPersonajeConfig & ) = 0 ;
} ;

/*** class ***
name :	   uu_UniformRandomSampler
derived :  uu_Samples
author :   Valerie Boor
date :     10/11/1999
see also : uu_MotionPlanner, uu_Sampler, GaussianSampler
description :
  Generates Samples uniform randomly and keeps a list of
  them in uu_UniformRandomSamples.Samples
*** end of class ***/
class uu_UniformRandomSampler : public uu_Sampler
{
  public:
    uu_UniformRandomSampler( const CProbot * Probot_p_m ) : uu_Sampler(Probot_p_m) {}
    int Generate(CPersonajeConfig & ) ;
} ;

/*** class ***
name :	   GaussianSampler
derived :  uu_Samples
author :   Valerie Boor
date :     10/11/1999
see also : uu_MotionPlanner, uu_Samples, UniformRandomSampler
description :
  Generates Samples uniform randomly and keeps a list of
  them in GaussianSamples.Samples
*** end of class ***/

class uu_GaussianSampler : public uu_Sampler
{

  public:
    uu_GaussianSampler( const CProbot * Probot_p_m ) : uu_Sampler(Probot_p_m) {}
    int Generate(CPersonajeConfig & ) ;
} ;

#endif

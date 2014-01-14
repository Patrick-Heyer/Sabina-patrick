#include "CSampler.h"
#include "CProbot.h"
#include "Constants.h"
#include "Random.h"
#include "uu_Utils.h"

// Two local functions for use in random sample generators

void GetRandomSample(CPersonajeConfig &c_a, CProbot * Probot_p_a )
{
  uu_Point Position_v ;
  switch(Probot_p_a->Parametros().ConfigurationType)
  {
    case PRM::TRANSLATING :
    {
      Position_v = c_a.Position() ;
      Position_v.X() = RandomBetween( Probot_p_a->Ambiente()->Workspace()->MinX(),
                             Probot_p_a->Ambiente()->Workspace()->MaxX() ) ;
      Position_v.Y() = RandomBetween( Probot_p_a->Ambiente()->Workspace()->MinY(),
                             Probot_p_a->Ambiente()->Workspace()->MaxY() ) ;
      Position_v.Z() = RandomBetween( Probot_p_a->Ambiente()->Workspace()->MinZ(),
                             Probot_p_a->Ambiente()->Workspace()->MaxZ() ) ;
      c_a.SetPosition(Position_v) ;

      Probot_p_a->Counters[PRM::NR_SAMPLES]++ ;        // A sample is generated
      break ;
    }
    case PRM::CAR_LIKE :    //z <- theta
    {
      Position_v.X() = RandomBetween( Probot_p_a->Ambiente()->Workspace()->MinX(),
                             Probot_p_a->Ambiente()->Workspace()->MaxX() ) ;
      Position_v.Y() = RandomBetween( Probot_p_a->Ambiente()->Workspace()->MinY(),
                             Probot_p_a->Ambiente()->Workspace()->MaxY() ) ;

      Position_v.Z() = 0.0 ; //RandomBetween(0.0,360.0 ) ; //grados

      c_a.SetPosition(Position_v) ;

      Probot_p_a->Counters[PRM::NR_SAMPLES]++ ;        // A sample is generated
      break ;
    }


    default :
    {
      uu_Utils::FlushConsole("ERROR: [Random Sampling]-: ARTICULATED & CAR_LIKE not implemented\n" ) ;
      break ;
    }
  }
}

//---------------------------------------------------------------------------
CPersonajeConfig GetNearbySample(const CPersonajeConfig &c_a, double Sigma_d_a,
                CProbot * Probot_p_a )
{
  uu_Point Position_v ;
  static CPersonajeConfig NewConf ;
  switch(Probot_p_a->Parametros().ConfigurationType)
  {
    case PRM::TRANSLATING :
    {
      Position_v.X() = c_a.Position().X() + NormalRand( Sigma_d_a ) ; ;
      Position_v.Y() = c_a.Position().Y() + NormalRand( Sigma_d_a ) ; ;
      Position_v.Z() = c_a.Position().Z() + NormalRand( Sigma_d_a ) ; ;
      NewConf.SetPosition(Position_v) ;

      Probot_p_a->Counters[PRM::NR_SAMPLES]++ ;        // A sample is generated
      break ;
    }
    case PRM::FREE_FLYING :
    {
      Position_v.X() = c_a.Position().X() + NormalRand( Sigma_d_a ) ; ;
      Position_v.Y() = c_a.Position().Y() + NormalRand( Sigma_d_a ) ; ;
      Position_v.Z() = c_a.Position().Z() + NormalRand( Sigma_d_a ) ; ;
      NewConf.SetPosition(Position_v) ;

      // TODO!? orientation is chosen randomly
      //NewConf.SetOrientation( c_a.Orientation().random() ) ;

      Probot_p_a->Counters[PRM::NR_SAMPLES]++ ;        // A sample is generated
      break ;
    }
    case PRM::CAR_LIKE :
    {
      Position_v.X() = c_a.Position().X() + NormalRand( Sigma_d_a ) ; 
      Position_v.Y() = c_a.Position().Y() + NormalRand( Sigma_d_a ) ; 
      Position_v.Z() =  0.0 ; //c_a.Position().Z() + NormalRand( Sigma_d_a ) ; ;
      NewConf.SetPosition(Position_v) ;

      // TODO!? orientation theat is chosen randomly
//      NewConf.SetOrientation( c_a.Orientation().randomtheta() ) ;

      Probot_p_a->Counters[PRM::NR_SAMPLES]++ ;        // A sample is generated
      break ;
    }


    default :
    {
      uu_Utils::FlushConsole("ERROR: [Gaussian Sampling]-: ARTICULATED & CAR_LIKE not implemented\n") ;
      break ;
    }
  }
  return NewConf ;
}

//---------------------------------------------------------------------------
/*** memberfunction ***
name :     uu_Sampler
class :    uu_Sampler
input :    uu_MotionPlanner *MP_p_a
output :   void
author :   Valerie Boor
date :     26/11/1999
see also : uu_Sampler
description :
  Constructor of uu_Sampler.
*** end of memberfunction ***/
uu_Sampler::uu_Sampler( const CProbot * Probot_p_a )
{
  _Probot_p_m = (CProbot *)Probot_p_a ;

}

/*** memberfunction ***
name :     Generate
class :    UniformRandomSampler
input :    uu_RobotConfig & NewConfig
output :   int
author :   Valerie Boor
date :     10/11/1999
see also : uu_Sampler
description :
  Generates a new uu_RobotConfig and puts it in the appropriate
  vector in UniformRandomSampler.Samples
*** end of memberfunction ***/
int  uu_UniformRandomSampler::Generate( CPersonajeConfig & NewConfig )
{
  CPersonajeConfig c1 ;
 

  GetRandomSample( c1, _Probot_p_m ) ;

  if ( (_Probot_p_m->Ambiente()->Colision( c1 )==0) )
  // c1 is a free configuration
  {
    NewConfig = c1 ;
    ((CSamples *)(_Probot_p_m->Samples()))->AddSample(PRM::FREE, c1 ) ;
    _Probot_p_m->Counters[PRM::FREE]++ ;

    return 1 ;
  }
  else
    {
      ((CSamples *)(_Probot_p_m->Samples()))->AddSample(PRM::FAILED, c1 ) ;
      _Probot_p_m->Counters[PRM::FAILED]++ ;
      return 0 ;
    }
}

/*** memberfunction ***
name :     Generat
class :    GaussianSampler
input :    uu_RobotConfig & NewConfig
output :   int
author :   Valerie Boor
date :     10/11/1999
see also : uu_Sampler
description :
  Generates a new uu_RobotConfig and puts it in the appropriate
  vector in GaussianSampler.Samples
*** end of memberfunction ***/

int  uu_GaussianSampler::Generate(CPersonajeConfig & NewConfig )
{
;
  CPersonajeConfig c1, c2 ;
  std::vector <CPersonajeConfig> PersonajeConfigs_v_v;

  GetRandomSample( c1, _Probot_p_m ) ;  // distributed" distance
  c2 = GetNearbySample( c1, _Probot_p_m->Parametros().Sigma, _Probot_p_m ) ; 
//  c2 = GetNearbySample( c1, 0.7, _Probot_p_m ) ; 

  while ( !c2.InWorkspace(_Probot_p_m) )
  {
    /////// tester; was: ////  _Probot_p_m->Counters[PRM::TEST] += ( !c2.InWorkspace(_Probot_p_m) ) ;
   _Probot_p_m->Counters[PRM::TEST]++ ;
                                      // this should count the number of
                                      // configurations that have accidentally
                                      // been sampled outside of the workspace
                                      // after which we have to take another
                                      // and do a new collision test etc...
    c2 = GetNearbySample( c1, _Probot_p_m->Parametros().Sigma, _Probot_p_m ) ;

  }

  if ( _Probot_p_m->Ambiente()->Colision( c1 )==0 )
  // c1 is a free configuration
  {
    _Probot_p_m->Counters[PRM::FREE]++ ;

    if ( _Probot_p_m->Ambiente()->Colision( c2 )==0 )
    // c2 is a free configuration
    {
       _Probot_p_m->Counters[PRM::FREE]++ ;

      ((CSamples *)(_Probot_p_m->Samples()))->AddSample( PRM::PAIRFREE, c1 ) ;
      ((CSamples *)(_Probot_p_m->Samples()))->AddSample( PRM::PAIRFREE, c2 ) ;
      // _Probot_p_m->Counters[PRM::FREE]++ ;
      // _Probot_p_m->Counters[PRM::FREE]++ ;
      _Probot_p_m->Counters[PRM::PAIRFREE]++ ;
      _Probot_p_m->Counters[PRM::PAIRFREE]++ ;

      return 0 ;
    }
    else        // c2 is a forbidden configuration
    {
      _Probot_p_m->Counters[PRM::FAILED]++ ;

      NewConfig = c1 ;
      ((CSamples *)(_Probot_p_m->Samples()))->AddSample( PRM::MIXEDFREE, c1 ) ;
      ((CSamples *)(_Probot_p_m->Samples()))->AddSample( PRM::MIXEDFAILED, c2 ) ;
      _Probot_p_m->Counters[PRM::MIXEDFREE]++ ;
      _Probot_p_m->Counters[PRM::MIXEDFAILED]++ ;
      _Probot_p_m->Counters[PRM::SAMPLES]++ ;
      _Probot_p_m->Counters[PRM::RESAMPLES]++ ;


      return 1 ;
    }
  }
  else          // c1 is a forbidden configuration
  {
    _Probot_p_m->Counters[PRM::FAILED]++ ;

    if ( _Probot_p_m->Ambiente()->Colision( c2 )==0 )
    // c2 is a free configuration
    {
      _Probot_p_m->Counters[PRM::FREE]++ ;

      NewConfig = c2 ;
      ((CSamples *)(_Probot_p_m->Samples()))->AddSample( PRM::MIXEDFREE, c2 ) ;
      ((CSamples *)(_Probot_p_m->Samples()))->AddSample( PRM::MIXEDFAILED, c1 ) ;
      _Probot_p_m->Counters[PRM::MIXEDFREE]++ ;
      _Probot_p_m->Counters[PRM::MIXEDFAILED]++ ;
      _Probot_p_m->Counters[PRM::SAMPLES]++ ;
      _Probot_p_m->Counters[PRM::RESAMPLES]++ ;


      return 1 ;
    }
    else        // c2 is a forbidden configuration
    {
      _Probot_p_m->Counters[PRM::FAILED]++ ;

      ((CSamples *)(_Probot_p_m->Samples()))->AddSample( PRM::PAIRFAILED, c1 ) ;
      ((CSamples *)(_Probot_p_m->Samples()))->AddSample( PRM::PAIRFAILED, c2 ) ;
      _Probot_p_m->Counters[PRM::PAIRFAILED]++ ;
      _Probot_p_m->Counters[PRM::PAIRFAILED]++ ;
      // _Probot_p_m->Counters[PRM::FAILED]++ ;
      // _Probot_p_m->Counters[PRM::FAILED]++ ;
      return 0 ;
    }
  }
}

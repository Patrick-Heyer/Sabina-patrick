//---------------------------------------------------------------------------
#include "CSamples.h"
#include "Constants.h"
#include "CPersonajeConfig.h"
//---------------------------------------------------------------------------


CSamples::CSamples()
{
    _Samples_m.clear() ;
    std::vector < CPersonajeConfig > Configs_v ;
    for (int i=0; i<PRM::NR_OF_TYPES_OF_SAMPLES; i++) _Samples_m.push_back(Configs_v) ;
}
//---------------------------------------------------------------------------

void CSamples::AddSample( int Index_i_a, const CPersonajeConfig & Config_r_a )
{
    _Samples_m[Index_i_a].push_back( Config_r_a ) ;
}

//---------------------------------------------------------------------------

void CSamples::Clear()
{
    for (int i=0; i<PRM::NR_OF_TYPES_OF_SAMPLES; i++) _Samples_m[i].clear() ;
}

//---------------------------------------------------------------------------

const CPersonajeConfig & CSamples::Sample( int Index_i_a, int Index2_i_a ) const
{
    return _Samples_m[Index_i_a][Index2_i_a] ;
}

//---------------------------------------------------------------------------

const std::vector < CPersonajeConfig > & CSamples::Samples( int Index_i_a ) const
{
    return _Samples_m[Index_i_a] ;
}

//---------------------------------------------------------------------------

int CSamples::Size() const
{
    return (int)(_Samples_m.size()) ;
}

//---------------------------------------------------------------------------

int CSamples::Size(int Index_i_a) const
{
    return (int)(_Samples_m[Index_i_a].size()) ;
}

//---------------------------------------------------------------------------

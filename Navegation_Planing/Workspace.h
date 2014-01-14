
#ifndef WorkspaceH
#define WorkspaceH

#include "TiposBasicos.h"

///////////////////////////////////////////////////////////////////////
///////////////////////// uu_Workspace ////////////////////////////////
///////////////////////////////////////////////////////////////////////

/*** class ***
name :     uu_Workspace
derived :
author :   Valerie Boor
date :     09/02/1999
see also :
description

*** end of class ***/
class CWorkspace
{
private:

    double __Minima_pd_m[3] ;
    double __Maxima_pd_m[3] ;

public:

    CWorkspace() ;
    CWorkspace( double MinX_d_a, double MinY_d_a, double MinZ_d_a,
                double MaxX_d_a, double MaxY_d_a, double MaxZ_d_a  ) ;
    ~CWorkspace() {}

    const double & MinX() const {
        return __Minima_pd_m[0] ;
    }
    const double & MinY() const {
        return __Minima_pd_m[1] ;
    }
    const double & MinZ() const {
        return __Minima_pd_m[2] ;
    }
    const double & MaxX() const {
        return __Maxima_pd_m[0] ;
    }
    const double & MaxY() const {
        return __Maxima_pd_m[1] ;
    }
    const double & MaxZ() const {
        return __Maxima_pd_m[2] ;
    }

    int PonerMinX( double X_d_a ) ;
    int PonerMinY( double Y_d_a ) ;
    int PonerMinZ( double Z_d_a ) ;
    int PonerMaxX( double X_d_a ) ;
    int PonerMaxY( double Y_d_a ) ;
    int PonerMaxZ( double Z_d_a ) ;
    int PonerMin( double X_d_a, double Y_d_a, double Z_d_a ) ;
    int PonerMax( double X_d_a, double Y_d_a, double Z_d_a ) ;
    int PonerMinMax( double MinX_d_a, double MinY_d_a, double MinZ_d_a,
                     double MaxX_d_a, double MaxY_d_a, double MaxZ_d_a  ) ;

    int Inside( const uu_Point & ) const ;
} ;

#endif

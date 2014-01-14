

#include "Workspace.h"


///////////////////////////////////////////////////////////////////////
///////////////////////// uu_Workspace ////////////////////////////////
///////////////////////////////////////////////////////////////////////

CWorkspace::CWorkspace()
{
  __Minima_pd_m[0] = 0.0 ;
  __Minima_pd_m[1] = 0.0 ;
  __Minima_pd_m[2] = 0.0 ;
  __Maxima_pd_m[0] = 1.0 ;
  __Maxima_pd_m[1] = 1.0 ;
  __Maxima_pd_m[2] = 1.0 ;
}


CWorkspace::CWorkspace( double MinX_d_a, double MinY_d_a,
                        double MinZ_d_a, double MaxX_d_a,
                        double MaxY_d_a, double MaxZ_d_a )
{
  __Minima_pd_m[0] = MinX_d_a ;
  __Minima_pd_m[1] = MinY_d_a ;
  __Minima_pd_m[2] = MinZ_d_a ;
  __Maxima_pd_m[0] = MaxX_d_a ;
  __Maxima_pd_m[1] = MaxY_d_a ;
  __Maxima_pd_m[2] = MaxZ_d_a ;
}


/*** memberfunction ***
name :     SetMinX
class :    uu_Workspace
input :    double X_d_a
output :   int
author :   Valerie Boor
date :     09/02/1999
see also :
description

*** end of memberfunction ***/
int  CWorkspace::PonerMinX( double X_d_a )
{
  __Minima_pd_m[0] = X_d_a ;
  return 1 ;
}


/*** memberfunction ***
name :     SetMinY
class :    uu_Workspace
input :    double Y_d_a
output :   int
author :   Valerie Boor
date :     09/02/1999
see also :
description

*** end of memberfunction ***/
int  CWorkspace::PonerMinY( double Y_d_a )
{
  __Minima_pd_m[1] = Y_d_a ;
  return 1 ;
}


/*** memberfunction ***
name :     SetMinZ
class :    uu_Workspace
input :    double Z_d_a
output :   int
author :   Valerie Boor
date :     09/02/1999
see also :
description

*** end of memberfunction ***/
int  CWorkspace::PonerMinZ( double Z_d_a )
{
  __Minima_pd_m[2] = Z_d_a ;
  return 1 ;
}


/*** memberfunction ***
name :     SetMaxX
class :    uu_Workspace
input :    double X_d_a
output :   int
author :   Valerie Boor
date :     09/02/1999
see also :
description

*** end of memberfunction ***/
int  CWorkspace::PonerMaxX( double X_d_a )
{
  __Maxima_pd_m[0] = X_d_a ;
  return 1 ;
}


/*** memberfunction ***
name :     SetMaxY
class :    uu_Workspace
input :    double Y_d_a
output :   int
author :   Valerie Boor
date :     09/02/1999
see also :
description

*** end of memberfunction ***/
int  CWorkspace::PonerMaxY( double Y_d_a )
{
  __Maxima_pd_m[1] = Y_d_a ;
  return 1 ;
}


/*** memberfunction ***
name :     SetMaxZ
class :    uu_Workspace
input :    double Z_d_a
output :   int
author :   Valerie Boor
date :     09/02/1999
see also :
description

*** end of memberfunction ***/
int  CWorkspace::PonerMaxZ( double Z_d_a )
{
  __Maxima_pd_m[2] = Z_d_a ;
  return 1 ;
}


/*** memberfunction ***
name :     SetMin
class :    uu_Workspace
input :    double X_d_a, double Y_d_a, double Z_d_a
output :   int
author :   Valerie Boor
date :     09/02/1999
see also :
description

*** end of memberfunction ***/
int  CWorkspace::PonerMin( double X_d_a, double Y_d_a, double Z_d_a )
{
  __Minima_pd_m[0] = X_d_a ;
  __Minima_pd_m[1] = Y_d_a ;
  __Minima_pd_m[2] = Z_d_a ;
  return 1 ;
}


/*** memberfunction ***
name :     SetMax
class :    uu_Workspace
input :    double X_d_a, double Y_d_a, double Z_d_a
output :   int
author :   Valerie Boor
date :     09/02/1999
see also :
description

*** end of memberfunction ***/
int  CWorkspace::PonerMax( double X_d_a, double Y_d_a, double Z_d_a )
{
  __Maxima_pd_m[0] = X_d_a ;
  __Maxima_pd_m[1] = Y_d_a ;
  __Maxima_pd_m[2] = Z_d_a ;
  return 1 ;
}


int CWorkspace::PonerMinMax( double MinX_d_a, double MinY_d_a,
                             double MinZ_d_a, double MaxX_d_a,
                             double MaxY_d_a, double MaxZ_d_a )
{
  __Minima_pd_m[0] = MinX_d_a ;
  __Minima_pd_m[1] = MinY_d_a ;
  __Minima_pd_m[2] = MinZ_d_a ;
  __Maxima_pd_m[0] = MaxX_d_a ;
  __Maxima_pd_m[1] = MaxY_d_a ;
  __Maxima_pd_m[2] = MaxZ_d_a ;
  return 1 ;
}


/*** memberfunction ***
name :     Inside
class :    uu_Workspace
input :    const uu_Point &P_a
output :   int
author :   Valerie Boor
date :     09/02/1999
see also :
description

*** end of memberfunction ***/
int  CWorkspace::Inside( const uu_Point &P_a ) const
{
  return
  (
    ( P_a.X() >= __Minima_pd_m[0] && P_a.X() <= __Maxima_pd_m[0] )
      &&
    ( P_a.Y() >= __Minima_pd_m[1] && P_a.Y() <= __Maxima_pd_m[1] )
      &&
    ( P_a.Z() >= __Minima_pd_m[2] && P_a.Z() <= __Maxima_pd_m[2] )
  ) ;
}

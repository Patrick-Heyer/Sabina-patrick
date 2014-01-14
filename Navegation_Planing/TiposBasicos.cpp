
#include <math.h>

#include "TiposBasicos.h"

/*** memberfunction ***
name :     Length()
struct :   uu_Point
input :    void
output :   double
author :   Valerie Boor
date :     08/30/1999
see also : SqLength()
description :
  When the uu_Point p is considered a vector,
  p.Length() returns the length of this vector.
*** end of struct ***/
double uu_Point::Length()
{
  return sqrt( X()*X() + Y()*Y() + Z()*Z() ) ;
}


/*** memberfunction ***
name :     SqLength()
struct :   uu_Point
input :    void
output :   double
author :   Valerie Boor
date :     08/30/1999
see also : Length()
description :
  When the uu_Point p is considered a vector,
  p.SqLength() returns the square of the length of this vector.
*** end of struct ***/
double uu_Point::SqLength()
{
  return X()*X() + Y()*Y() + Z()*Z() ;
}


uu_Point operator+( uu_Point P, uu_Point Q )
{
  uu_Point Result ;
  Result.X() = P.X() + Q.X() ;
  Result.Y() = P.Y() + Q.Y() ;
  Result.Z() = P.Z() + Q.Z() ;
  return Result ;
}


uu_Point operator-( uu_Point P, uu_Point Q )
{
  uu_Point Result ;
  Result.X() = P.X() - Q.X() ;
  Result.Y() = P.Y() - Q.Y() ;
  Result.Z() = P.Z() - Q.Z() ;
  return Result ;
}



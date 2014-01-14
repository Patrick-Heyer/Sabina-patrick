#ifndef TiposBasicosH
#define TiposBasicosH

typedef struct { double Element[4][4] ; } uu_HomogeneousMatrix ;


/*** struct ***
name :	   uu_Point
derived :
author :   Valerie Boor
date :     08/30/1999
see also :
description :
  consists of a point in space, for the time being in 3D
*** end of struct ***/
struct uu_Point
{
  double P[3] ;
  double X() const { return P[0] ; }
  double &X() { return P[0] ; } // Nu kan: p.X() = 5.8
  double Y() const { return P[1] ; }
  double &Y() { return P[1] ; }
  double Z() const { return P[2] ; }
  double &Z() { return P[2] ; }

  double Length() ; // Length when considered a vector.
  double SqLength() ; // Square of the length when considered a vector.
} ;

/*** operator ***
name :	   +
struct :   uu_Point
author :   Valerie Boor
date :     08/30/1999
see also : -
description :
  When uu_Point p and q are considered vectors,
  p+q returns the vector sum of both.
*** end of operator ***/
uu_Point operator+( uu_Point p, uu_Point q ) ;

/*** operator ***
name :	   -
struct :   uu_Point
author :   Valerie Boor
date :     08/30/1999
see also : +
description :
  When uu_Point p and q are considered vectors,
  p-q returns the vector difference of both.
*** end of operator ***/
uu_Point operator-( uu_Point p, uu_Point q ) ;


#endif

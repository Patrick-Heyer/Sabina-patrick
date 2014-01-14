
///////////////////////////////////////////////////////////////
// Contains the random functions and distributions	     //
// By Boor. 						     //
///////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "Random.h"


/////////////////////////////////////////////////
// Given two floats, random_between returns a
// value in between these two floats.
/////////////////////////////////////////////////
double RandomBetween( double a, double b )
{
   double i ;
   i = (double)rand() / (double)RAND_MAX ;   // rand() returns a random
   i *= b - a ;			                 // int between 0 and RAND_MAX.
   return i + a ;
}


/////////////////////////////////////////////////
// Given two floats (or doubles), normal_rand
// returns a value in between these two floats.
/////////////////////////////////////////////////
double NormalRand( double Sigma )
{
  double a = RandomBetween( 0.0, 1.0 ) ;
  while(a == 0.0) a = RandomBetween( 0.0, 1.0 ) ;
  double b = RandomBetween( 0.0, 1.0 ) ;
  return Sigma*cos(2.0*M_PI*b)*sqrt(-2.0*log(a)) ;
}

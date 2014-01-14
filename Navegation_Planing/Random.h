
///////////////////////////////////////////////////////////////
// Contains the random functions and distributions	     //
// By Boor. 						     //
///////////////////////////////////////////////////////////////

#if !defined RANDOM_BOOR
#define RANDOM_BOOR 1

/*** function ***
name :     RandomBetween 
input :    double a, double b  
output :   double 
author :   Valerie Boor 
date :     09/02/1999 
see also : 
description 
  Given two doubles, random_between returns a
  value in between these two doubles.
*** end of function ***/
double RandomBetween( double a, double b ) ;

/*** function ***
name :     NormalRand 
input :    double Sigma  
output :   double 
author :   Valerie Boor 
date :     09/02/1999 
see also : 
description 
  Given two doubles, normal_rand
  returns a value in between these two doubles.
  drand48 is a uniform random number between 0 and 1
*** end of function ***/
double NormalRand( double Sigma ) ;

#endif

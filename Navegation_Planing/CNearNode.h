
#ifndef CNearNodeH
#define CNearNodeH

///////////////////////////////////////////////////////////////////////
////////////////////////// NearNode ///////////////////////////////////
///////////////////////////////////////////////////////////////////////

/*** class ***
name :     NearNode
derived :
author :   Valerie Boor
date :     09/02/1999
see also :
description

*** end of class ***/
class NearNode
{
  public:
  int i ;
  double d ;

  NearNode( ) ;
  NearNode( int index, double distance) ;
  int Index() const ;
  double Distance() const ;
} ;

#endif

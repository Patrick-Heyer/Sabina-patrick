//--------------------------------------------------------------------
#include "CNearNode.h"
//--------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////
////////////////////////// NearNode ///////////////////////////////////
///////////////////////////////////////////////////////////////////////

NearNode::NearNode( ) : i(0), d(0) {}


NearNode::NearNode(int index, double distance) : i(index), d(distance) {}


/*** memberfunction ***
name :     Index 
class :    NearNode 
input :    
output :   int  
author :   Valerie Boor 
date :     09/02/1999 
see also : 
description 

*** end of memberfunction ***/
int  NearNode::Index() const
{
  return i ;
}


/*** memberfunction ***
name :     Distance
class :    NearNode
input :
output :   double
author :   Valerie Boor
date :     09/02/1999
see also :
description

*** end of memberfunction ***/
double  NearNode::Distance() const
{
  return d ;
}



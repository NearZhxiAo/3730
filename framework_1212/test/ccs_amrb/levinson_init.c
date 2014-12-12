/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#include "levinson.h"
#include "levinson_reset.h"
 
/*************************************************************************
*
*  Function:   Levinson_init
*  Purpose:    Allocates state memory and initializes state memory
*
**************************************************************************
*/
int Levinson_init (LevinsonState *state)
{
  
  Levinson_reset(state);
  
  return 0;
}

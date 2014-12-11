/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#include "typedef.h"
#include "cnst.h"
#include "levinson.h"
 
/*************************************************************************
*
*  Function:   Levinson_reset
*  Purpose:    Initializes state memory to zero
*
**************************************************************************
*/
int Levinson_reset (LevinsonState *state)
{
  Word16 i;
  
  state->old_A[0] = 4096;
  for(i = 1; i < M + 1; i++)
      state->old_A[i] = 0;
 
  return 0;
}
 


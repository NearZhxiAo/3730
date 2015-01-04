/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"
#include "q_plsf.h"
 
/*
**************************************************************************
*
*  Function    : Q_plsf_reset
*  Purpose     : Resets state memory
*
**************************************************************************
*/
int Q_plsf_reset (Q_plsfState *state)
{
  Word16 i;
  
  for ( i = 0; i < M; i++)
      state->past_rq[i] = 0;
  
  return 0;
}
 

/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#include "pre_process.h"
 
/*************************************************************************
*
*  Function:   Pre_Process_reset
*  Purpose:    Initializes state memory to zero
*
**************************************************************************
*/
int Pre_Process_reset (Pre_ProcessState *state)
{
  
  state->y2_hi = 0;
  state->y2_lo = 0;
  state->y1_hi = 0;
  state->y1_lo = 0;
  state->x0 = 0;
  state->x1 = 0;
 
  return 0;
}

/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#include "gc_pred.h"
#include "gc_pred_reset.h"

/*************************************************************************
*
*  Function:   qua_gain_init
*  Purpose:    Allocates state memory and initializes state memory
*
**************************************************************************
*/
int gc_pred_init (gc_predState *state)
{
  gc_pred_reset(state);
  return 0;
}

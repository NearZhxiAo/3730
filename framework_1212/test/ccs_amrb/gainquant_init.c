/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#include <stdio.h>
#include "gainquant.h"
#include "gain_adapt_init.h"
#include "gainquant_reset.h"
#include "gc_pred_init.h"

/*************************************************************************
*
*  Function:   gainQuant_init
*  Purpose:    Allocates state memory and initializes state memory
*
**************************************************************************
*/
int gainQuant_init (gainQuantState *state)
{
  
  /* Init sub states */
  gc_pred_init(state->gc_predSt);
  gc_pred_init(state->gc_predUnqSt);
  gain_adapt_init(state->adaptSt);

  gainQuant_reset(state);
  
  return 0;
}

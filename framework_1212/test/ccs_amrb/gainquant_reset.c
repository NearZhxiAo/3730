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
#include "gc_pred.h"
#include "copy.h"
#include "set_zero.h"
#include "gc_pred_reset.h"

#include "gain_adapt_reset.h"
#include "gainquant.h"

/*************************************************************************
*
*  Function:   gainQuant_reset
*  Purpose:    Initializes state memory to zero
*
**************************************************************************
*/
int gainQuant_reset (gainQuantState *state)
{
  state->sf0_exp_gcode0 = 0;
  state->sf0_frac_gcode0 = 0;
  state->sf0_exp_target_en = 0;
  state->sf0_frac_target_en = 0;
  
  Set_zero (state->sf0_exp_coeff, 5);
  Set_zero (state->sf0_frac_coeff, 5);
  state->gain_idx_ptr = NULL;
  
  gc_pred_reset(state->gc_predSt);
  gc_pred_reset(state->gc_predUnqSt);
  gain_adapt_reset(state->adaptSt);
  
  return 0;
}

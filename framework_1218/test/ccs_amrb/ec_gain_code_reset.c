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
#include "ec_gain_code.h"

/*
**************************************************************************
*
*  Function    : ec_gain_code_reset
*  Purpose     : Resets state memory
*
**************************************************************************
*/
int ec_gain_code_reset (ec_gain_codeState *state)
{
  Word16 i;
  
  for ( i = 0; i < 5; i++)
      state->gbuf[i] = 1;
  state->past_gain_code = 0;
  state->prev_gc = 1;       

  return 0;
}

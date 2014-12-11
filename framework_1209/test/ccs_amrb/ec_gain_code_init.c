/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#include "ec_gain_code.h"
#include "ec_gain_code_reset.h"
/*
**************************************************************************
*
*  Function    : ec_gain_code_init
*  Purpose     : Allocates memory and initializes state variables
*
**************************************************************************
*/
int ec_gain_code_init (ec_gain_codeState *state)
{
  ec_gain_code_reset(state);
  
  return 0;
}

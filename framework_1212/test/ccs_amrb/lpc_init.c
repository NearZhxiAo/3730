/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/

#include "lpc.h"
#include "levinson_init.h"
#include "lpc_reset.h"

/*************************************************************************
*
*  Function:   lpc_init
*
**************************************************************************
*/
int lpc_init (lpcState *state)
{
  
  /* Init sub states */
  Levinson_init(state->levinsonSt);

  lpc_reset(state);
  
  return 0;
}

/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#include "q_plsf.h"
#include "q_plsf_reset.h"

/*
**************************************************************************
*
*  Function    : Q_plsf_init
*  Purpose     : Allocates memory and initializes state variables
*
**************************************************************************
*/
int Q_plsf_init (Q_plsfState *state)
{
  Q_plsf_reset(state);
  
  return 0;
}

/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#include "d_plsf.h"
#include "d_plsf_reset.h"

/*
**************************************************************************
*
*  Function    : D_plsf_init
*  Purpose     : Allocates and initializes state memory
*
**************************************************************************
*/
int D_plsf_init (D_plsfState *state)
{

  D_plsf_reset(state);
  
  return 0;
}

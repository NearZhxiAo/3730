/*************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#include "bgn_scd.h"
#include "bgn_scd_reset.h"

/*
**************************************************************************
*
*  Function    : Bgn_scd_init
*  Purpose     : Allocates and initializes state memory
*
**************************************************************************
*/
Word16 Bgn_scd_init (Bgn_scdState *state)
{
   Bgn_scd_reset(state);
  
   return 0;
}


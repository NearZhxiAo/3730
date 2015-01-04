/*************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/

#include "bgn_scd.h"
#include "set_zero.h"

/*
**************************************************************************
*
*  Function    : Bgn_scd_reset
*  Purpose     : Resets state memory
*
**************************************************************************
*/
Word16 Bgn_scd_reset (Bgn_scdState *state)
{

   /* Static vectors to zero */
   Set_zero (state->frameEnergyHist, L_ENERGYHIST);

   /* Initialize hangover handling */
   state->bgHangover = 0;
   
   return 0;
}


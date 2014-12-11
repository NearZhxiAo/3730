/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#include "typedef.h"
#include "gc_pred.h"

/*
*****************************************************************************
*                         LOCAL VARIABLES AND TABLES
*****************************************************************************
*/

/* MA prediction coefficients (Q6)  */

/* minimum quantized energy: -14 dB */
#define MIN_ENERGY       -14336       /* 14                 Q10 */
#define MIN_ENERGY_MR122  -2381       /* 14 / (20*log10(2)) Q10 */ 
/*************************************************************************
*
*  Function:   gc_pred_reset
*  Purpose:    Initializes state memory to zero
*
**************************************************************************
*/
int gc_pred_reset (gc_predState *state)
{
   Word16 i;

   for(i = 0; i < NPRED; i++)
   {
      state->past_qua_en[i] = MIN_ENERGY;
      state->past_qua_en_MR122[i] = MIN_ENERGY_MR122;
   }
  return 0;
}

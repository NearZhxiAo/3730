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
#include "basic_op.h"
#include "oper_32b.h"
#include "cnst.h"
#include "table.h"
#include "pseudonoise.h"
/*
********************************************************************************
*                         LOCAL CONSTANTS
********************************************************************************
*/
#define  NB_PULSE10 10 /* number of random pulses in DTX operation   */
/***************************************************************************
*
*  Function    : build_CN_code
*
***************************************************************************/ 
void build_CN_code (
    Word32 *seed,         /* i/o : Old CN generator shift register state */
    Word16 cod[]          /* o   : Generated CN fixed codebook vector    */
)
{
   Word16 i, j, k;
   
   for (i = 0; i < L_SUBFR; i++)
   {
      cod[i] = 0;                         
   }
   
   for (k = 0; k < NB_PULSE10; k++)
   {
      i = pseudonoise (seed, 2);      /* generate pulse position */
      i = shr (extract_l (L_mult (i, 10)), 1);
      i = i + k;
      
      j = pseudonoise (seed, 1);      /* generate sign           */

         
      if (j > 0)
      {
         cod[i] = 4096;                                 
      }
      else
      {
         cod[i] = -4096;                                      
      }
   }
   
   return;
}


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
#include "basic_op.h"
#include "ton_stab.h"

/***************************************************************************
 *
 *  Function:   Check_Gp_Clipping()                                          
 *  Purpose:    Verify that the sum of the last (N_FRAME+1) pitch  
 *              gains is under a certain threshold.              
 *                                                                         
 ***************************************************************************
 */ 
Word16 check_gp_clipping(tonStabState *st, /* i/o : State struct            */
                         Word16 g_pitch    /* i   : pitch gain              */
)
{
   Word16 i, sum;
   
   sum = g_pitch >> 3;          /* Division by 8 */
   for (i = 0; i < N_FRAME; i++)
   {
      sum = add(sum, st->gp[i]);
   }

   
   if (sum > GP_CLIP)
   {
      return 1;
   }
   else
   {
      return 0;
   }
}


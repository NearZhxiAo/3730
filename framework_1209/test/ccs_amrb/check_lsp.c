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
 *                                                                          *
 *  Function:  check_lsp()                                                  *
 *  Purpose:   Check the LSP's to detect resonances                         *
 *                                                                          *
 ****************************************************************************
 */
Word16 check_lsp(tonStabState *st, /* i/o : State struct            */
                 Word16 *lsp       /* i   : unquantized LSP's       */
)
{
   Word16 i, dist, dist_min1, dist_min2, dist_th;
 
   /* Check for a resonance:                             */
   /* Find minimum distance between lsp[i] and lsp[i+1]  */
 
   dist_min1 = MAX_16;                       
   for (i = 3; i < M-2; i++)
   {
      dist = sub(lsp[i], lsp[i+1]);

      
      if (dist < dist_min1)
      {
         dist_min1 = dist;                   
      }
   }

   dist_min2 = MAX_16;                       
   for (i = 1; i < 3; i++)
   {
      dist = sub(lsp[i], lsp[i+1]);

      
      if (dist < dist_min2)
      {
         dist_min2 = dist;                   
      }
   }

   if ( lsp[1] > 32000)
   {
      dist_th = 600;                         
   }
   else if (lsp[1] > 30500)
   {
      dist_th = 800;                         
   }
   else
   {
      dist_th = 1100;                        
   }

    
   if ((dist_min1 < 1500) ||
       (dist_min2 < dist_th))
   {
      st->count = st->count + 1;
   }
   else
   {
      st->count = 0;                         
   }
   
   /* Need 12 consecutive frames to set the flag */
   
   if (st->count >= 12)
   {
      st->count = 12;                        
      return 1;
   }
   else
   {
      return 0;
   }
}

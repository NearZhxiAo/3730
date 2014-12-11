/*
*****************************************************************************
**-------------------------------------------------------------------------**
**                                                                         **
**     GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001       **
**                               R99   Version 3.2.0                       **
**                               REL-4 Version 4.0.0                       **
**                                                                         **
**-------------------------------------------------------------------------**
*****************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "cnst_vad.h"
#include "vad1.h"
/****************************************************************************
 *
 *     Function   : complex_estimate_adapt
 *     Purpose    : Update/adapt of complex signal estimate
 *     Inputs     : low_power:   low signal power flag 
 *     Outputs    : st->corr_hp_fast:   long term complex signal estimate
 *
 ***************************************************************************/
void complex_estimate_adapt(
         vadState1 *st,       /* i/o : VAD state struct                       */
         Word16 low_power    /* i   : very low level flag of the input frame */
         )
{
   Word16 alpha;            /* Q15 */
   Word32 L_tmp;            /* Q31 */


   /* adapt speed on own state */
   
   if (sub(st->best_corr_hp, st->corr_hp_fast) < 0) /* decrease */
   {
      
      if (sub(st->corr_hp_fast, CVAD_THRESH_ADAPT_HIGH) < 0)
      {  /* low state  */
         alpha = CVAD_ADAPT_FAST;                           
      }  
      else 
      {  /* high state */
         alpha = CVAD_ADAPT_REALLY_FAST;                      
      }      
   }
   else  /* increase */ 
   {
      
      if (sub(st->corr_hp_fast, CVAD_THRESH_ADAPT_HIGH) < 0)
      {  
         alpha = CVAD_ADAPT_FAST;                           
      }  
      else 
      {  
         alpha = CVAD_ADAPT_SLOW;                          
      }      
   }

   L_tmp = L_deposit_h(st->corr_hp_fast);
   L_tmp = L_msu(L_tmp, alpha, st->corr_hp_fast);
   L_tmp = L_mac(L_tmp, alpha, st->best_corr_hp);
   st->corr_hp_fast = round(L_tmp);           /* Q15 */       

   
   if (sub(st->corr_hp_fast, CVAD_MIN_CORR) <  0)
   {
      st->corr_hp_fast = CVAD_MIN_CORR;                    
   }

   
   if (low_power != 0)
   {
      st->corr_hp_fast = CVAD_MIN_CORR;                    
   }   
}

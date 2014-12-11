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
#include "complex_estimate_adapt.h"
#include "complex_vad.h"
#include "noise_estimate_update.h"
#include "hangover_addition.h"
/****************************************************************************
 *
 *     Function     : vad_decision
 *     Purpose      : Calculates VAD_flag
 *     Inputs       : bckr_est:    background noise estimate
 *                    vadreg:      intermediate VAD flags
 *     Outputs      : noise_level: average level of the noise estimates
 *                    vadreg:      intermediate VAD flags
 *     Return value : VAD_flag
 *
 ***************************************************************************/
Word16 vad_decision(
             vadState1 *st,          /* i/o : State struct                       */
             Word16 level[COMPLEN], /* i   : sub-band levels of the input frame */
             Word32 pow_sum         /* i   : power of the input frame           */
             )
{
   Word16 i;
   Word16 snr_sum;
   Word32 L_temp;
   Word16 vad_thr, temp, noise_level;
   Word16 low_power_flag;
   
   /* 
      Calculate squared sum of the input levels (level)
      divided by the background noise components (bckr_est).
      */
   L_temp = 0;                                            
   for (i = 0; i < COMPLEN; i++)
   {
      Word16 exp;
      
      exp = norm_s(st->bckr_est[i]);
      temp = shl(st->bckr_est[i], exp);
      temp = div_s(shr(level[i], 1), temp);
      temp = shl(temp, sub(exp, UNIRSHFT-1));
      L_temp = L_mac(L_temp, temp, temp);
   }
   snr_sum = extract_h(L_shl(L_temp, 6));
   snr_sum = mult(snr_sum, INV_COMPLEN);

   /* Calculate average level of estimated background noise */
   L_temp = 0;                                            
   for (i = 0; i < COMPLEN; i++)
   {
      L_temp = L_add(L_temp, st->bckr_est[i]);
   }
   
   noise_level = extract_h(L_shl(L_temp, 13));
   
   /* Calculate VAD threshold */
   vad_thr = add(mult(VAD_SLOPE, sub(noise_level, VAD_P1)), VAD_THR_HIGH);
   
   
   if (vad_thr < VAD_THR_LOW)
   {
      vad_thr = VAD_THR_LOW;                              
   }
   
   /* Shift VAD decision register */
   st->vadreg = st->vadreg >> 1;                       
   
   /* Make intermediate VAD decision */
   
   if (snr_sum > vad_thr)
   {
      st->vadreg = st->vadreg | 0x4000;        
   }
   /* primary vad decsion made */
   
   /* check if the input power (pow_sum) is lower than a threshold" */
   
   if (pow_sum < VAD_POW_LOW)
   {
      low_power_flag = 1;                                 
   }
   else
   {
      low_power_flag = 0;                                 
   }
   
   /* update complex signal estimate st->corr_hp_fast and hangover reset timer using */
   /* low_power_flag and corr_hp_fast  and various adaptation speeds                 */
   complex_estimate_adapt(st, low_power_flag);

   /* check multiple thresholds of the st->corr_hp_fast value */
   st->complex_warning = complex_vad(st, low_power_flag);     

   /* Update speech subband vad background noise estimates */
   noise_estimate_update(st, level);
     
   /*  Add speech and complex hangover and return speech VAD_flag */
   /*  long term complex hangover may be added */
   st->speech_vad_decision = hangover_addition(st, noise_level, low_power_flag);
   
   
   return (st->speech_vad_decision);
}


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
 *     Function     : hangover_addition
 *     Purpose      : Add hangover for complex signal or after speech bursts
 *     Inputs       : burst_count:  counter for the length of speech bursts
 *                    hang_count:   hangover counter
 *                    vadreg:       intermediate VAD decision
 *     Outputs      : burst_count:  counter for the length of speech bursts
 *                    hang_count:   hangover counter
 *     Return value : VAD_flag indicating final VAD decision
 *
 ***************************************************************************/
Word16 hangover_addition(
              vadState1 *st,       /* i/o : State struct                     */
              Word16 noise_level, /* i   : average level of the noise       */
                                  /*       estimates                        */
              Word16 low_power    /* i   : flag power of the input frame    */
              )
{
   Word16 hang_len, burst_len;
   
   /* 
      Calculate burst_len and hang_len
      burst_len: number of consecutive intermediate vad flags with "1"-decision
                 required for hangover addition
      hang_len:  length of the hangover
      */

   
   if (sub(noise_level, HANG_NOISE_THR) > 0)
   {
      burst_len = BURST_LEN_HIGH_NOISE;                           
      hang_len = HANG_LEN_HIGH_NOISE;                             
   }
   else
   {
      burst_len = BURST_LEN_LOW_NOISE;                            
      hang_len = HANG_LEN_LOW_NOISE;                              
   }
   
   /* if the input power (pow_sum) is lower than a threshold, clear
      counters and set VAD_flag to "0"  "fast exit"                 */
   
   if (low_power != 0)
   {
      st->burst_count = 0;                                        
      st->hang_count = 0;                                         
      st->complex_hang_count = 0;                                 
      st->complex_hang_timer = 0;                                 
      return 0;
   }
   
   
   if (sub(st->complex_hang_timer, CVAD_HANG_LIMIT) > 0)
   {
      
      if (sub(st->complex_hang_count, CVAD_HANG_LENGTH) < 0)
      {
         st->complex_hang_count = CVAD_HANG_LENGTH;               
      }      
   }
   
   /* long time very complex signal override VAD output function */
   
   if (st->complex_hang_count != 0)
   {
      st->burst_count = BURST_LEN_HIGH_NOISE;                     
      st->complex_hang_count = sub(st->complex_hang_count, 1);    
      return 1; 
   }
   else
   {
      /* let hp_corr work in from a noise_period indicated by the VAD */
        
      if (((st->vadreg & 0x3ff0) == 0) &&
          (sub(st->corr_hp_fast, CVAD_THRESH_IN_NOISE) > 0))
      {
         return 1;
      }  
   }

   /* update the counters (hang_count, burst_count) */
    
   if ((st->vadreg & 0x4000) != 0)
   {
      st->burst_count = add(st->burst_count, 1);                  
      
      if (sub(st->burst_count, burst_len) >= 0)
      {
         st->hang_count = hang_len;                               
      }
      return 1;
   }
   else
   {
      st->burst_count = 0;                                        
      
      if (st->hang_count > 0)
      {
         st->hang_count = sub(st->hang_count, 1);                 
         return 1;
      }
   }
   return 0;
}

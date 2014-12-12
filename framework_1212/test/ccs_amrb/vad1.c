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
#include "filter_bank.h"
#include "vad_decision.h"

/****************************************************************************
 *
 *     Function     : vad
 *     Purpose      : Main program for Voice Activity Detection (VAD) for AMR 
 *     Return value : VAD Decision, 1 = speech, 0 = noise
 *
 ***************************************************************************/
Word16 vad1(vadState1 *st,      /* i/o : State struct                 */
           Word16 in_buf[]    /* i   : samples of the input frame   */
           )
{
   Word16 level[COMPLEN];
   Word32 pow_sum;
   Word16 i;
   
   /* Calculate power of the input frame. */
   pow_sum = 0L;                                     

   for (i = 0; i < FRAME_LEN; i++)
   {  
      pow_sum = L_mac(pow_sum, in_buf[i-LOOKAHEAD], in_buf[i-LOOKAHEAD]);
   }

   /*
     If input power is very low, clear pitch flag of the current frame
     */
   
   if (L_sub(pow_sum, POW_PITCH_THR) < 0)
   {
      st->pitch = st->pitch & 0x3fff;                 
   }

   /*
     If input power is very low, clear complex flag of the "current" frame
     */
   
   if (L_sub(pow_sum, POW_COMPLEX_THR) < 0)
   {
      st->complex_low = st->complex_low & 0x3fff;     
   }
   
   /*
     Run the filter bank which calculates signal levels at each band
     */
   filter_bank(st, in_buf, level);
   
   return (vad_decision(st, level, pow_sum));
}

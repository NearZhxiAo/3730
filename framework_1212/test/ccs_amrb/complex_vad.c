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
 *     Function     : complex_vad
 *     Purpose      : complex background decision
 *     Return value : the complex background decision
 *
 ***************************************************************************/
Word16 complex_vad(vadState1 *st,    /* i/o : VAD state struct              */
                          Word16 low_power /* i   : flag power of the input frame */
                          )
{
   st->complex_high = shr(st->complex_high, 1);                      
   st->complex_low = shr(st->complex_low, 1);                        

   
   if (low_power == 0)
   {
      
      if (sub(st->corr_hp_fast, CVAD_THRESH_ADAPT_HIGH) > 0)
      {
         st->complex_high = st->complex_high | 0x4000;    
      }
      
      
      if (sub(st->corr_hp_fast, CVAD_THRESH_ADAPT_LOW) > 0 )
      {
         st->complex_low = st->complex_low | 0x4000;      
      }
   }

   
   if (sub(st->corr_hp_fast, CVAD_THRESH_HANG) > 0)
   {
      st->complex_hang_timer = add(st->complex_hang_timer, 1);       
   }
   else
   {
      st->complex_hang_timer =  0;                                   
   }               
   
      
   return ((sub((st->complex_high & 0x7f80), 0x7f80) == 0) ||
           (sub((st->complex_low & 0x7fff), 0x7fff) == 0));
}

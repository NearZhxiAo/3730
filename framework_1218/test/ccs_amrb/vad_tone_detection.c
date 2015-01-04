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
 *     Function     : vad_tone_detection
 *     Purpose      : Set tone flag if pitch gain is high. This is used to detect
 *                    signaling tones and other signals with high pitch gain.
 *     Inputs       : tone: flags indicating presence of a tone
 *     Outputs      : tone: flags indicating presence of a tone
 *
 ***************************************************************************/
void vad_tone_detection (vadState1 *st,  /* i/o : State struct            */
                         Word32 t0,     /* i   : autocorrelation maxima  */
                         Word32 t1      /* i   : energy                  */
                         )
{
   Word16 temp;
   /* 
      if (t0 > TONE_THR * t1)
      set tone flag
      */
   temp = round(t1);
   
    
   if ((temp > 0) && (L_msu(t0, temp, TONE_THR) > 0))
   {
      st->tone = st->tone | 0x4000;               
   }
}


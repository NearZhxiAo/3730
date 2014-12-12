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
#include "vad1.h"
/****************************************************************************
 *
 *     Function     : vad_pitch_detection
 *     Purpose      : Test whether signal contains pitch or other periodic
 *                    component.
 *     Return value : Boolean voiced / unvoiced decision in state variable 
 *
 ***************************************************************************/
void vad_pitch_detection (vadState1 *st,   /* i/o : State struct                  */
                          Word16 T_op[]   /* i   : speech encoder open loop lags */
                          )
{
   Word16 lagcount, i;
   
   lagcount = 0;               
   
   for (i = 0; i < 2; i++)
   {
      
      if (sub (abs_s (sub (st->oldlag, T_op[i])), LTHRESH) < 0)
      {
         lagcount = lagcount + 1;
      }
      
      /* Save the current LTP lag */
      st->oldlag = T_op[i];       
   }
   
   /* Make pitch decision.
      Save flag of the pitch detection to the variable pitch.
      */
   st->pitch = st->pitch >> 1; 
   
   
   if (sub ( add (st->oldlag_count, lagcount), NTHRESH) >= 0)
   {
      st->pitch = st->pitch | 0x4000;  
   }
   
   /* Update oldlagcount */
   st->oldlag_count = lagcount;     
}



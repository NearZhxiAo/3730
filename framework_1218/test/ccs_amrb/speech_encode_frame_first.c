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
#include "speech_encode_frame.h"
#include "pre_process.h"
#include "cod_amr_first.h"

int Speech_Encode_Frame_First (
    Speech_Encode_FrameState *st,  /* i/o : post filter states       */
    Word16 *new_speech)            /* i   : speech input             */
{
#if !defined(NO13BIT)
   Word16 i;
#endif

#if !defined(NO13BIT)
  /* Delete the 3 LSBs (13-bit input) */
  for (i = 0; i < L_NEXT; i++) 
  {
     new_speech[i] = new_speech[i] & 0xfff8;     
  }
#endif

  /* filter + downscaling */
  Pre_Process (st->pre_state, new_speech, L_NEXT);

  cod_amr_first(st->cod_amr_state, new_speech);

  return 0;
}

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
#include "cnst.h"
#include "set_zero.h"
#include "mode.h"

#include "pre_process.h"
#include "prm2bits.h"
#include "cod_amr.h"
#include "speech_encode_frame.h"

int Speech_Encode_Frame (
    Speech_Encode_FrameState *st, /* i/o : post filter states          */
    enum Mode mode,               /* i   : speech coder mode           */
    Word16 *new_speech,           /* i   : speech input                */
    Word16 *serial,               /* o   : serial bit stream           */
    enum Mode *usedMode           /* o   : used speech coder mode */
    )
{
  Word16 prm[MAX_PRM_SIZE];   /* Analysis parameters.                  */
  Word16 syn[L_FRAME];        /* Buffer for synthesis speech           */
  Word16 i;

  /* initialize the serial output frame to zero */
  for (i = 0; i < MAX_SERIAL_SIZE; i++)   
  {
    serial[i] = 0;                                           
  }

#if !defined(NO13BIT)
  /* Delete the 3 LSBs (13-bit input) */
  for (i = 0; i < L_FRAME; i++)   
  {
     new_speech[i] = new_speech[i] & 0xfff8;     
  }
#endif

  /* filter + downscaling */
  Pre_Process (st->pre_state, new_speech, L_FRAME);           
  
  /* Call the speech encoder */
  cod_amr(st->cod_amr_state, mode, new_speech, prm, usedMode, syn);
  
  /* Parameters to serial bits */
  Prm2bits (*usedMode, prm, &serial[0]); 

  return 0;
}

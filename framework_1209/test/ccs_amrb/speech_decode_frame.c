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
#include "decoder_amr.h"
#include "bits2prm.h"
#include "mode.h"
#include "post_process.h"
#include "speech_decode_frame.h"

int Speech_Decode_Frame (
    Speech_Decode_FrameState *st, /* io: post filter states                */
    enum Mode mode,               /* i : AMR mode                          */
    Word16 *serial,               /* i : serial bit stream                 */
    enum RXFrameType frame_type,    /* i : Frame type                        */
    Word16 *synth                 /* o : synthesis speech (postfiltered    */
                                  /*     output)                           */
)
{
  Word16 parm[MAX_PRM_SIZE + 1];  /* Synthesis parameters                */
  Word16 Az_dec[AZ_SIZE];         /* Decoded Az for post-filter          */
                                  /* in 4 subframes                      */

#if !defined(NO13BIT)
  Word16 i;
#endif

  /* Serial to parameters   */
  if ((frame_type == RX_SID_BAD) ||
      (frame_type == RX_SID_UPDATE)) {
    /* Override mode to MRDTX */
    Bits2prm (MRDTX, serial, parm);
  } else {
    Bits2prm (mode, serial, parm);
  }

  /* Synthesis */
  Decoder_amr(st->decoder_amrState, mode, parm, frame_type,
              synth, Az_dec);

  Post_Filter(st->post_state, mode, synth, Az_dec);   /* Post-filter */

  /* post HP filter, and 15->16 bits */
  Post_Process(st->postHP_state, synth, L_FRAME);  
  
#if !defined(NO13BIT)
  /* Truncate to 13 bits */
  for (i = 0; i < L_FRAME; i++) 
  {
     synth[i] = synth[i] & 0xfff8;     
  }
#endif
  
  return 0;
}

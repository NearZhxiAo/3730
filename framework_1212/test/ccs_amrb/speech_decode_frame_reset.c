/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#include "speech_decode_frame.h"
#include "decoder_amr_reset.h"
#include "post_filter_reset.h"
#include "post_process_reset.h"
 
/*************************************************************************
*
*  Function:   Speech_Decode_Frame_reset
*  Purpose:    Resetses state memory
*
**************************************************************************
*/
int Speech_Decode_Frame_reset (Speech_Decode_FrameState *state)
{
  Decoder_amr_reset(state->decoder_amrState, (enum Mode)0);
  Post_Filter_reset(state->post_state);
  Post_Process_reset(state->postHP_state);

  state->prev_mode = (enum Mode)0;

  return 0;
}

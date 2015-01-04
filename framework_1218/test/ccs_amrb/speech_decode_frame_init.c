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
#include "decoder_amr_init.h"
#include "post_filter_init.h"
#include "post_process_init.h"
#include "speech_decode_frame_reset.h"
/*************************************************************************
*
*  Function:   Speech_Decode_Frame_init
*  Purpose:    Allocates memory for filter structure and initializes
*              state memory
*
**************************************************************************
*/
int Speech_Decode_Frame_init (Speech_Decode_FrameState *state)
{
  Decoder_amr_init(state->decoder_amrState);
  Post_Filter_init(state->post_state);
  Post_Process_init(state->postHP_state);
  
  Speech_Decode_Frame_reset(state);
  
  return 0;
}

/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#ifndef Speech_Decode_Frame_h
#define Speech_Decode_Frame_h "$Id $"
 
#include "mode.h"
#include "decoder_amr.h"
#include "post_filter.h"
#include "post_process.h"

typedef struct{
  Decoder_amrState* decoder_amrState;
  Post_FilterState*  post_state;
  Post_ProcessState* postHP_state;
  enum Mode prev_mode;

} Speech_Decode_FrameState;
 
int Speech_Decode_Frame (
    Speech_Decode_FrameState *st, /* io: post filter states                */
    enum Mode mode,               /* i : AMR mode                          */
    Word16 *serial,               /* i : serial bit stream                 */
    enum RXFrameType frame_type,  /* i : Frame type                        */
    Word16 *synth                 /* o : synthesis speech (postfiltered    */
                                  /*     output)                           */
);
 
#endif

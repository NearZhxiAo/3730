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
#include "pre_process_init.h"
#include "cod_amr_init.h"
#include "pre_process_reset.h"
#include "cod_amr_reset.h"
#include "speech_encode_frame.h" 
#include "speech_encode_frame_reset.h"

/*************************************************************************
*
*  Function:   Speech_Encode_Frame_init
*  Purpose:    Allocates memory for filter structure and initializes
*              state memory
*
**************************************************************************
*/
int Speech_Encode_Frame_init (Speech_Encode_FrameState *state,
                              Flag dtx)
{

  state->dtx = dtx;

  Pre_Process_init(state->pre_state);
  cod_amr_init(state->cod_amr_state, state->dtx);

  Speech_Encode_Frame_reset(state);
  
  return 0;
}

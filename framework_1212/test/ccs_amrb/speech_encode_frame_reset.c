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
#include "cod_amr_first.h"
#include "speech_encode_frame.h"
 
/*************************************************************************
*
*  Function:   Speech_Encode_Frame_reset
*  Purpose:    Resetses state memory
*
**************************************************************************
*/
int Speech_Encode_Frame_reset (Speech_Encode_FrameState *state)
{
  
  Pre_Process_reset(state->pre_state);
  cod_amr_reset(state->cod_amr_state);

  return 0;
}

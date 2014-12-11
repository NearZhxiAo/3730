/*************************************************************************
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
#include "copy.h"
#include "set_zero.h"
#include "frame.h"
#include "table.h"
#include "decoder_amr.h"
#include "d_plsf_reset.h"
#include "ec_gain_pitch_reset.h"
#include "ec_gain_code_reset.h"
#include "cb_gain_average_reset.h"
#include "bgn_scd_reset.h"
#include "ph_disp_reset.h"
#include "dtx_dec_reset.h"
#include "lsp_avg_reset.h"
#include "gc_pred_reset.h"
/*
**************************************************************************
*
*  Function    : Decoder_amr_reset
*  Purpose     : Resets state memory
*
**************************************************************************
*/
int Decoder_amr_reset (Decoder_amrState *state, enum Mode mode)
{
  Word16 i;  
  /* Initialize static pointer */
  state->exc = state->old_exc + PIT_MAX + L_INTERPOL;

  /* Static vectors to zero */
  Set_zero (state->old_exc, PIT_MAX + L_INTERPOL);

  if (mode != MRDTX)
     Set_zero (state->mem_syn, M);

  /* initialize pitch sharpening */
  state->sharp = SHARPMIN;
  state->old_T0 = 40;
     
  /* Initialize state->lsp_old [] */ 

  if (mode != MRDTX) {
      Copy(lsp_init_data, &state->lsp_old[0], M);
  }

  /* Initialize memories of bad frame handling */
  state->prev_bf = 0;
  state->prev_pdf = 0;
  state->state = 0;
  
  state->T0_lagBuff = 40;
  state->inBackgroundNoise = 0;
  state->voicedHangover = 0;
  if (mode != MRDTX) {
      for (i=0;i<9;i++)
          state->excEnergyHist[i] = 0;
  }
  
  for (i = 0; i < 9; i++)
     state->ltpGainHistory[i] = 0;

  Cb_gain_average_reset(state->Cb_gain_averState);
  if (mode != MRDTX)
     lsp_avg_reset(state->lsp_avg_st);
  D_plsf_reset(state->lsfState);
  ec_gain_pitch_reset(state->ec_gain_p_st);
  ec_gain_code_reset(state->ec_gain_c_st);

  if (mode != MRDTX)
     gc_pred_reset(state->pred_state);

  Bgn_scd_reset(state->background_state);
  state->nodataSeed = 21845;
  ph_disp_reset(state->ph_disp_st);
  if (mode != MRDTX)
     dtx_dec_reset(state->dtxDecoderState);
  
  return 0;
}

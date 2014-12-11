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
#include "syn_filt.h"
#include "d_plsf.h"
#include "agc.h"
#include "int_lpc_1and3.h"
#include "int_lpc_1to3.h"
#include "dec_gain.h"
#include "dec_lag3.h"
#include "dec_lag6.h"
#include "pred_lt_3or6.h"
#include "dec_gain.h"
#include "ph_disp.h"
#include "int_lsf.h"
#include "lsp_lsf.h"
#include "lsp_avg.h"
#include "ex_ctrl.h"
#include "sqrt_l_exp.h"
#include "frame.h"

#include "table.h"
#include "gc_pred_init.h"
#include "gc_pred_reset.h"
#include "lsp_avg_init.h"
#include "lsp_avg_reset.h"
#include "lsf_lsp.h"
#include "decoder_amr.h"
#include "d_plsf_init.h"
#include "ec_gain_pitch_init.h"
#include "ec_gain_code_init.h"
#include "cb_gain_average_init.h"
#include "Bgn_scd_init.h"
#include "ph_disp_init.h"
#include "dtx_dec_init.h"
#include "decoder_amr_reset.h"

/*
**************************************************************************
*
*  Function    : Decoder_amr_init
*  Purpose     : Allocates and initializes state memory
*
**************************************************************************
*/
int Decoder_amr_init (Decoder_amrState *state)
{
  Word16 i;
 
  state->T0_lagBuff = 40;
  state->inBackgroundNoise = 0;
  state->voicedHangover = 0;
  for (i = 0; i < 9; i++)
     state->ltpGainHistory[i] = 0;

  D_plsf_init(state->lsfState);
  ec_gain_pitch_init(state->ec_gain_p_st);
  ec_gain_code_init(state->ec_gain_c_st);
  gc_pred_init(state->pred_state);
  Cb_gain_average_init(state->Cb_gain_averState);
  lsp_avg_init(state->lsp_avg_st);
  Bgn_scd_init(state->background_state);
  ph_disp_init(state->ph_disp_st);
  dtx_dec_init(state->dtxDecoderState);
      
  Decoder_amr_reset(state, (enum Mode)0);

  
  return 0;
}


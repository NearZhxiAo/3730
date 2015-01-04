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
#include "copy.h"
#include "set_zero.h"
#include "cl_ltp_init.h"
#include "lsp_init.h"
#include "gainquant_init.h"
#include "dtx_enc_init.h"
#include "p_ol_wgh_init.h"
#include "lpc_init.h"
#include "cod_amr_reset.h"
#include "ton_stab_init.h"
#include "vad1_init.h"

/*
**************************************************************************
*
*  Function    : cod_amr_init
*  Purpose     : Allocates memory and initializes state variables
*
**************************************************************************
*/
int cod_amr_init (cod_amrState *state, Flag dtx)
{

  state->dtx = dtx;
  
  /* Init sub states */
  cl_ltp_init(state->clLtpSt);
  lsp_init(state->lspSt);
  gainQuant_init(state->gainQuantSt);
  p_ol_wgh_init(state->pitchOLWghtSt);
  ton_stab_init(state->tonStabSt);
  vad1_init(state->vadSt);
  dtx_enc_init(state->dtx_encSt);
  lpc_init(state->lpcSt);
  
  cod_amr_reset(state);
  
  return 0;
}

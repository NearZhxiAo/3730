/*************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#include "cnst.h"
#include "set_zero.h"
#include "agc_reset.h"
#include "preemphasis_reset.h"
#include "post_filter.h"

/*************************************************************************
*
*  Function:   Post_Filter_reset
*  Purpose:    Initializes state memory to zero
*
**************************************************************************
*/
int Post_Filter_reset (Post_FilterState *state)
{
  
  Set_zero (state->mem_syn_pst, M);
  Set_zero (state->res2, L_SUBFR);
  Set_zero (state->synth_buf, L_FRAME + M);
  agc_reset(state->agc_state);
  preemphasis_reset(state->preemph_state);

  return 0;
}

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
#include "table.h"
#include "cnst.h"
#include "copy.h"
#include "d_plsf.h"
 
/*
**************************************************************************
*
*  Function    : D_plsf_reset
*  Purpose     : Resets state memory
*
**************************************************************************
*/
int D_plsf_reset (D_plsfState *state)
{
  Word16 i;
  
  for (i = 0; i < M; i++){
      state->past_r_q[i] = 0;             /* Past quantized prediction error */
  }
  
  /* Past dequantized lsfs */
  Copy(mean_lsf_5, &state->past_lsf_q[0], M);

  return 0;
}
 

/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/

#include "dtx_def.h"
#include "typedef.h"
#include "copy.h"
#include "set_zero.h"
#include "cnst.h"
#include "table.h"
/*
**************************************************************************
*
*  Function    : dtx_enc_reset
*
**************************************************************************
*/
int dtx_enc_reset (dtx_encState *st)
{
  Word16 i;


  st->hist_ptr = 0;
  st->log_en_index = 0;
  st->init_lsf_vq_index = 0;
  st->lsp_index[0] = 0;
  st->lsp_index[1] = 0;
  st->lsp_index[2] = 0;
 
  /* Init lsp_hist[] */
  for(i = 0; i < DTX_HIST_SIZE; i++)
  {
    Copy(lsp_init_data, &st->lsp_hist[i * M], M);
  }

  /* Reset energy history */
  Set_zero(st->log_en_hist, M);

  st->dtxHangoverCount = DTX_HANG_CONST;
  st->decAnaElapsedCount = 32767; 

  return 1;
}

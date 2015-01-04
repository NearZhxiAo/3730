/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#include "copy.h"
#include "cnst.h"
#include "table.h"

#include "lsp.h"
#include "q_plsf_reset.h"
/*
**************************************************************************
*
*  Function    : lsp_reset
*
**************************************************************************
*/
int lsp_reset (lspState *st)
{
  /* Init lsp_old[] */
  Copy(lsp_init_data, &st->lsp_old[0], M);

  /* Initialize lsp_old_q[] */
  Copy(st->lsp_old, st->lsp_old_q, M);
  
  /* Reset quantization state */
   Q_plsf_reset(st->qSt);

  return 0;
}

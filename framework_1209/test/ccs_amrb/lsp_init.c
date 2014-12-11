/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#include "lsp.h"
#include "lsp_reset.h"
#include "q_plsf_init.h"
/*
**************************************************************************
*
*  Function    : lsp_init
*
**************************************************************************
*/
int lsp_init (lspState *st)
{
  /* Initialize quantization state */
   Q_plsf_init(st->qSt);

  lsp_reset(st);
  
  return 0;
}

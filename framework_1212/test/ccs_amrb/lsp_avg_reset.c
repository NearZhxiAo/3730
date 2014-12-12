/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#include "lsp_avg.h"
#include "table.h"
#include "copy.h"
#include "cnst.h"
/*
**************************************************************************
*
*  Function    : lsp_avg_reset
*  Purpose     : Resets state memory
*
**************************************************************************
*/
int lsp_avg_reset (lsp_avgState *st)
{ 

  Copy(mean_lsf_5, &st->lsp_meanSave[0], M);
  
  return 0;
}
 



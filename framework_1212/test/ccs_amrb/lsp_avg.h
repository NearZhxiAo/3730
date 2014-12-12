/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef lsp_avg_h
#define lsp_avg_h "$Id $"
 
#include "typedef.h"
#include "cnst.h"

#define EXPCONST          5243               /* 0.16 in Q15 */
 
/*
********************************************************************************
*                         DEFINITION OF DATA TYPES
********************************************************************************
*/
typedef struct {
  Word16 lsp_meanSave[M];          /* Averaged LSPs saved for efficiency  */
} lsp_avgState;
 
 
/*
**************************************************************************
*
*  Function    : lsp_avg
*  Purpose     : Calculate the LSP averages
*
**************************************************************************
*/
void lsp_avg (
    lsp_avgState *st,     /* i/o : State struct                 Q15 */
    Word16 *lsp           /* i   : LSP vector                   Q15 */
);
#endif

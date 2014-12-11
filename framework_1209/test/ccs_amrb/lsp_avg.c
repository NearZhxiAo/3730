/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#include "basic_op.h"
#include "oper_32b.h"
#include "table.h"
#include "copy.h"
#include "lsp_avg.h"

/*
**************************************************************************
*
*  Function    : lsp_avg
*  Purpose     : Calculate the LSP averages
*
**************************************************************************
*/

void lsp_avg (
    lsp_avgState *st,         /* i/o : State struct                 Q15 */
    Word16 *lsp               /* i   : state of the state machine   Q15 */
)
{
    Word16 i;
    Word32 L_tmp;            /* Q31 */

    for (i = 0; i < M; i++) {

       /* mean = 0.84*mean */
       L_tmp = L_deposit_h(st->lsp_meanSave[i]);
       L_tmp = L_msu(L_tmp, EXPCONST, st->lsp_meanSave[i]);

       /* Add 0.16 of newest LSPs to mean */
       L_tmp = L_mac(L_tmp, EXPCONST, lsp[i]);

       /* Save means */
       st->lsp_meanSave[i] = round(L_tmp);             /* Q15 */
    }

    return;
}

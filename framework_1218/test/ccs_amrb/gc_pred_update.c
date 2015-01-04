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
#include "gc_pred.h"

/*************************************************************************
 *
 * FUNCTION:  gc_pred_update()
 *
 * PURPOSE: update MA predictor with last quantized energy
 *
 *************************************************************************/
void gc_pred_update(
    gc_predState *st,      /* i/o: State struct                     */
    Word16 qua_ener_MR122, /* i  : quantized energy for update, Q10 */
                           /*      (log2(qua_err))                  */
    Word16 qua_ener        /* i  : quantized energy for update, Q10 */
                           /*      (20*log10(qua_err))              */
)
{
    Word16 i;

    for (i = 3; i > 0; i--)
    {
        st->past_qua_en[i] = st->past_qua_en[i - 1];             
        st->past_qua_en_MR122[i] = st->past_qua_en_MR122[i - 1]; 
    }

    st->past_qua_en_MR122[0] = qua_ener_MR122;  /*    log2 (qua_err), Q10 */
	                                                             
    st->past_qua_en[0] = qua_ener;              /* 20*log10(qua_err), Q10 */
	                                                             
}

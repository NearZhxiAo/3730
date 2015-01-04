/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "cnst.h"
#include "gmed_n.h"
#include "gc_pred.h"
#include "table.h"
#include "gc_pred_average_limited.h"
#include "gc_pred_update.h"
#include "table.h"
#include "ec_gain_code.h"

/*
**************************************************************************
*
*  Function    : ec_gain_code
*  Purpose     : conceal the codebook gain
*                Call this function only in BFI (instead of normal gain
*                decoding function)
*
**************************************************************************
*/
void ec_gain_code (
    ec_gain_codeState *st,    /* i/o : State struct                     */
    gc_predState *pred_state, /* i/o : MA predictor state               */
    Word16 state,             /* i   : state of the state machine       */
    Word16 *gain_code         /* o   : decoded innovation gain          */
)
{
    Word16 tmp;
    Word16 qua_ener_MR122;
    Word16 qua_ener;
    
    /* calculate median of last five gain values */
    tmp = gmed_n (st->gbuf,5);                                 

    /* new gain = minimum(median, past_gain) * cdown[state] */
     
    if (sub (tmp, st->past_gain_code) > 0)
    {
        tmp = st->past_gain_code;                               
    }
    tmp = mult (tmp, cdown[state]);
    *gain_code = tmp;                                           

    /* update table of past quantized energies with average of
     * current values
     */
    gc_pred_average_limited(pred_state, &qua_ener_MR122, &qua_ener);
    gc_pred_update(pred_state, qua_ener_MR122, qua_ener);
}


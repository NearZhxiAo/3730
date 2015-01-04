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
*  Function    : ec_gain_code_update
*  Purpose     : update the codebook gain concealment state;
*                limit gain_code if the previous frame was bad
*                Call this function always after decoding (or concealing)
*                the gain
*
**************************************************************************
*/
void ec_gain_code_update (
    ec_gain_codeState *st,    /* i/o : State struct                     */
    Word16 bfi,               /* i   : flag: frame is bad               */
    Word16 prev_bf,           /* i   : flag: previous frame was bad     */
    Word16 *gain_code         /* i/o : decoded innovation gain          */
)
{
    Word16 i;
    
    /* limit gain_code by previous good gain if previous frame was bad */
    
    if (bfi == 0)
    {
		
        if (prev_bf != 0)
        {
             
            if (sub (*gain_code, st->prev_gc) > 0)
            {
                *gain_code = st->prev_gc;      
            }
        }
        st->prev_gc = *gain_code;                           
    }

    /* update EC states: previous gain, gain buffer */
    st->past_gain_code = *gain_code;                        
    
    for (i = 1; i < 5; i++)
    {
        st->gbuf[i - 1] = st->gbuf[i];                      
    }
    st->gbuf[4] = *gain_code;                               

    return;
}


/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#include "ec_gain_pitch.h"
#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "cnst.h"
#include "table.h"
#include "gmed_n.h"

/*
**************************************************************************
*
*  Function    : ec_gain_pitch
*  Purpose     : conceal the pitch gain
*                Call this function only in BFI (instead of normal gain
*                decoding function)
*
**************************************************************************
*/
void ec_gain_pitch (
    ec_gain_pitchState *st, /* i/o : state variables                   */
    Word16 state,           /* i   : state of the state machine        */
    Word16 *gain_pitch      /* o   : pitch gain (Q14)                  */
)
{

    Word16 tmp;

    /* calculate median of last five gains */
    tmp = gmed_n (st->pbuf, 5);                         

    /* new gain = minimum(median, past_gain) * pdown[state] */
     
    if (sub (tmp, st->past_gain_pit) > 0)
    {
        tmp = st->past_gain_pit;                        
    }
    *gain_pitch = mult (tmp, pdown[state]);
}


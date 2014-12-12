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


/*
**************************************************************************
*
*  Function    : ec_gain_pitch_update
*  Purpose     : update the pitch gain concealment state;
*                limit gain_pitch if the previous frame was bad
*                Call this function always after decoding (or concealing)
*                the gain
*
**************************************************************************
*/
void ec_gain_pitch_update (
    ec_gain_pitchState *st, /* i/o : state variables                   */
    Word16 bfi,             /* i   : flag: frame is bad                */
    Word16 prev_bf,         /* i   : flag: previous frame was bad      */
    Word16 *gain_pitch      /* i/o : pitch gain                        */
)
{
    Word16 i;

     
    if (bfi == 0)
    {
        
        if (prev_bf != 0)
        {
             
            if (sub (*gain_pitch, st->prev_gp) > 0)
            {
                *gain_pitch = st->prev_gp;
            }
        }
        st->prev_gp = *gain_pitch;                          
    }
    
    st->past_gain_pit = *gain_pitch;                       

     
    if (sub (st->past_gain_pit, 16384) > 0)  /* if (st->past_gain_pit > 1.0) */
    {
        st->past_gain_pit = 16384;                          
    }
    for (i = 1; i < 5; i++)
    {
        st->pbuf[i - 1] = st->pbuf[i];                      
    }
    st->pbuf[4] = st->past_gain_pit;                        
}

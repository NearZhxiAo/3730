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
#include "basic_op.h"
#include "cnst.h"
#include "inv_sqrt.h"
#include "agc.h"
#include "energy_new.h"
/*
**************************************************************************
*
*  Function    : agc
*  Purpose     : Scales the postfilter output on a subframe basis
*
**************************************************************************
*/
int agc (
    agcState *st,      /* i/o : agc state                        */
    Word16 *sig_in,    /* i   : postfilter input signal  (l_trm) */
    Word16 *sig_out,   /* i/o : postfilter output signal (l_trm) */
    Word16 agc_fac,    /* i   : AGC factor                       */
    Word16 l_trm       /* i   : subframe size                    */
)
{
    Word16 i, exp;
    Word16 gain_in, gain_out, g0, gain;
    Word32 s;
            
    /* calculate gain_out with exponent */
    s = energy_new(sig_out, l_trm);  /* function result */
        
     
    if (s == 0)
    {
        st->past_gain = 0;           
        return 0;
    }
    exp = sub (norm_l (s), 1);
    gain_out = round (L_shl (s, exp));

    /* calculate gain_in with exponent */
    s = energy_new(sig_in, l_trm);    /* function result */
    
     
    if (s == 0)
    {
        g0 = 0;                  
    }
    else
    {
        i = norm_l (s);
        gain_in = round (L_shl (s, i));
        exp = sub (exp, i);

        /*---------------------------------------------------*
         *  g0 = (1-agc_fac) * sqrt(gain_in/gain_out);       *
         *---------------------------------------------------*/

        s = L_deposit_l (div_s (gain_out, gain_in));
        s = L_shl (s, 7);       /* s = gain_out / gain_in */
        s = L_shr (s, exp);     /* add exponent */

        s = Inv_sqrt (s);  /* function result */
        i = round (L_shl (s, 9));

        /* g0 = i * (1-agc_fac) */
        g0 = mult (i, sub (32767, agc_fac));
    }

    /* compute gain[n] = agc_fac * gain[n-1]
                        + (1-agc_fac) * sqrt(gain_in/gain_out) */
    /* sig_out[n] = gain[n] * sig_out[n]                        */

    gain = st->past_gain;            

    for (i = 0; i < l_trm; i++)
    {
        gain = mult (gain, agc_fac);
        gain = add (gain, g0);
        sig_out[i] = extract_h (L_shl (L_mult (sig_out[i], gain), 3));
                                 
    }

    st->past_gain = gain;            

    return 0;
}

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
#include "cnst.h"
#include "table.h"
/*************************************************************************
 *
 *  FUNCTION:   Pred_lt_3or6()
 *
 *  PURPOSE:  Compute the result of long term prediction with fractional
 *            interpolation of resolution 1/3 or 1/6. (Interpolated past
 *            excitation).
 *
 *  DESCRIPTION:
 *       The past excitation signal at the given delay is interpolated at
 *       the given fraction to build the adaptive codebook excitation.
 *       On return exc[0..L_subfr-1] contains the interpolated signal
 *       (adaptive codebook excitation).
 *
 *************************************************************************/
void Pred_lt_3or6 (
    Word16 exc[],     /* in/out: excitation buffer                         */
    Word16 T0,        /* input : integer pitch lag                         */
    Word16 frac,      /* input : fraction of lag                           */
    Word16 L_subfr,   /* input : subframe size                             */
    Word16 flag3      /* input : if set, upsampling rate = 3 (6 otherwise) */
)
{
    Word16 i, j, k;
    Word16 *x0, *x1, *x2;
    const Word16 *c1, *c2;
    Word32 s;

    x0 = &exc[-T0];              

    frac = negate (frac);
    
    if (flag3 != 0)
    {
      frac = shl (frac, 1);   /* inter_3l[k] = inter_6[2*k] -> k' = 2*k */
    }
    
     
    if (frac < 0)
    {
        frac = frac + UP_SAMP_MAX;
        x0--;
    }

    for (j = 0; j < L_subfr; j++)
    {
        x1 = x0++;               
        x2 = x0;                 
        c1 = &inter_6[frac];
        c2 = &inter_6[sub (UP_SAMP_MAX, frac)];

        s = 0;                   
        for (i = 0, k = 0; i < L_INTER10; i++, k += UP_SAMP_MAX)
        {
            s = L_mac (s, x1[-i], c1[k]);
            s = L_mac (s, x2[i], c2[k]);
        }

        exc[j] = round (s);      
    }

    return;
}

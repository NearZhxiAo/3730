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

#define UP_SAMP_MAX  6

/*************************************************************************
 *
 *  FUNCTION:  Interpol_3or6()
 *
 *  PURPOSE:  Interpolating the normalized correlation with 1/3 or 1/6
 *            resolution.
 *
 *************************************************************************/
Word16 Interpol_3or6 (  /* o : interpolated value                        */
    Word16 *x,          /* i : input vector                              */
    Word16 frac,        /* i : fraction  (-2..2 for 3*, -3..3 for 6*)    */
    Word16 flag3        /* i : if set, upsampling rate = 3 (6 otherwise) */
)
{
    Word16 i, k;
    Word16 *x1, *x2;
    const Word16 *c1, *c2;
    Word32 s;

    
    if (flag3 != 0)
    {
      frac = shl (frac, 1);   /* inter_3[k] = inter_6[2*k] -> k' = 2*k */
    }
    
     
    if (frac < 0)
    {
        frac = add (frac, UP_SAMP_MAX);
        x--;
    }
    
    x1 = &x[0];                          
    x2 = &x[1];                          
    c1 = &inter_3or6[frac];                 
    c2 = &inter_3or6[sub (UP_SAMP_MAX, frac)];  

    s = 0;                               
    for (i = 0, k = 0; i < L_INTER_SRCH; i++, k += UP_SAMP_MAX)
    {
        s = L_mac (s, x1[-i], c1[k]);
        s = L_mac (s, x2[i], c2[k]);
    }

    return round (s);
}

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

void Weight_Ai (
    Word16 a[],         /* (i)     : a[M+1]  LPC coefficients   (M=10)    */
    const Word16 fac[], /* (i)     : Spectral expansion factors.          */
    Word16 a_exp[]      /* (o)     : Spectral expanded LPC coefficients   */
)
{
    Word16 i;

    a_exp[0] = a[0];                                     
    for (i = 1; i <= M; i++)
    {
        a_exp[i] = round (L_mult (a[i], fac[i - 1]));    
    }

    return;
}

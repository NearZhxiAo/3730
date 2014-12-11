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
#include "get_lsp_pol.h"
/*************************************************************************
 *
 *  FUNCTION:   Lsp_Az
 *
 *  PURPOSE:  Converts from the line spectral pairs (LSP) to
 *            LP coefficients, for a 10th order filter.
 *
 *  DESCRIPTION:
 *     - Find the coefficients of F1(z) and F2(z) (see Get_lsp_pol)
 *     - Multiply F1(z) by 1+z^{-1} and F2(z) by 1-z^{-1}
 *     - A(z) = ( F1(z) + F2(z) ) / 2
 *
 *************************************************************************/
void Lsp_Az (
    Word16 lsp[],        /* (i)  : line spectral frequencies            */
    Word16 a[]           /* (o)  : predictor coefficients (order = 10)  */
)
{
    Word16 i, j;
    Word32 f1[6], f2[6];
    Word32 t0;

    Get_lsp_pol (&lsp[0], f1);
    Get_lsp_pol (&lsp[1], f2);

    for (i = 5; i > 0; i--)
    {
        f1[i] = L_add (f1[i], f1[i - 1]);     /* f1[i] += f1[i-1]; */
        f2[i] = L_sub (f2[i], f2[i - 1]);     /* f2[i] -= f2[i-1]; */
    }

    a[0] = 4096;                              
    for (i = 1, j = 10; i <= 5; i++, j--)
    {
        t0 = L_add (f1[i], f2[i]);           /* f1[i] + f2[i] */
        a[i] = extract_l (L_shr_r (t0, 13));  
        t0 = L_sub (f1[i], f2[i]);           /* f1[i] - f2[i] */
        a[j] = extract_l (L_shr_r (t0, 13));  
    }

    return;
}


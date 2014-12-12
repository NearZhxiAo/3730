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
/*************************************************************************
 *
 *  FUNCTION:  Get_lsp_pol
 *
 *  PURPOSE:  Find the polynomial F1(z) or F2(z) from the LSPs.
 *            If the LSP vector is passed at address 0  F1(z) is computed
 *            and if it is passed at address 1  F2(z) is computed.
 *
 *  DESCRIPTION:
 *       This is performed by expanding the product polynomials:
 *
 *           F1(z) =   product   ( 1 - 2 lsp[i] z^-1 + z^-2 )
 *                   i=0,2,4,6,8
 *           F2(z) =   product   ( 1 - 2 lsp[i] z^-1 + z^-2 )
 *                   i=1,3,5,7,9
 *
 *       where lsp[] is the LSP vector in the cosine domain.
 *
 *       The expansion is performed using the following recursion:
 *
 *            f[0] = 1
 *            b = -2.0 * lsp[0]
 *            f[1] = b
 *            for i=2 to 5 do
 *               b = -2.0 * lsp[2*i-2];
 *               f[i] = b*f[i-1] + 2.0*f[i-2];
 *               for j=i-1 down to 2 do
 *                   f[j] = f[j] + b*f[j-1] + f[j-2];
 *               f[1] = f[1] + b;
 *
 *************************************************************************/

void Get_lsp_pol (Word16 *lsp, Word32 *f)
{
    Word16 i, j, hi, lo;
    Word32 t0;
    
    /* f[0] = 1.0;             */
    *f = L_mult (4096, 2048);               
    f++;                                    
    *f = L_msu ((Word32) 0, *lsp, 512);    /* f[1] =  -2.0 * lsp[0];  */
    f++;                                    
    lsp += 2;                              /* Advance lsp pointer     */

    for (i = 2; i <= 5; i++)
    {
        *f = f[-2];                         

        for (j = 1; j < i; j++, f--)
        {
            L_Extract (f[-1], &hi, &lo);
            t0 = Mpy_32_16 (hi, lo, *lsp); /* t0 = f[-1] * lsp    */
            t0 = L_shl (t0, 1);
            *f = L_add (*f, f[-2]);         /* *f += f[-2]      */
            *f = L_sub (*f, t0); /* *f -= t0            */
        }
        *f = L_msu (*f, *lsp, 512);         /* *f -= lsp<<9     */
        f += i;                            /* Advance f pointer   */
        lsp += 2;                          /* Advance lsp pointer */
    }

    return;
}

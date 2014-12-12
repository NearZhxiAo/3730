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

/*
**************************************************************************
*
*  Function    : Chebps
*  Purpose     : Evaluates the Chebyshev polynomial series
*  Description : - The polynomial order is   n = m/2 = 5
*                - The polynomial F(z) (F1(z) or F2(z)) is given by
*                   F(w) = 2 exp(-j5w) C(x)
*                  where
*                   C(x) = T_n(x) + f(1)T_n-1(x) + ... +f(n-1)T_1(x) + f(n)/2
*                  and T_m(x) = cos(mw) is the mth order Chebyshev
*                  polynomial ( x=cos(w) )
*  Returns     : C(x) for the input x.
*
**************************************************************************
*/
Word16 Chebps (
			   Word16 x,
			   Word16 f[], /* (n) */
			   Word16 n)
{
    Word16 i, cheb;
    Word16 b0_h, b0_l, b1_h, b1_l, b2_h, b2_l;
    Word32 t0;

    b2_h = 256;                    /* b2 = 1.0 */
    b2_l = 0;                      

    t0 = L_mult (x, 512);          /* 2*x                 */
    t0 = L_mac (t0, f[1], 8192);   /* + f[1]              */
    L_Extract (t0, &b1_h, &b1_l);  /* b1 = 2*x + f[1]     */

    for (i = 2; i < n; i++)
    {
        t0 = Mpy_32_16 (b1_h, b1_l, x);         /* t0 = 2.0*x*b1        */
        t0 = L_shl (t0, 1);
        t0 = L_mac (t0, b2_h, (Word16) 0x8000); /* t0 = 2.0*x*b1 - b2   */
        t0 = L_msu (t0, b2_l, 1);
        t0 = L_mac (t0, f[i], 8192);            /* t0 = 2.0*x*b1 - b2 + f[i] */

        L_Extract (t0, &b0_h, &b0_l);           /* b0 = 2.0*x*b1 - b2 + f[i]*/

        b2_l = b1_l;                 /* b2 = b1; */
        b2_h = b1_h;                
        b1_l = b0_l;                /* b1 = b0; */
        b1_h = b0_h;                
    }

    t0 = Mpy_32_16 (b1_h, b1_l, x);             /* t0 = x*b1; */
    t0 = L_mac (t0, b2_h, (Word16) 0x8000);     /* t0 = x*b1 - b2   */
    t0 = L_msu (t0, b2_l, 1);
    t0 = L_mac (t0, f[i], 4096);                /* t0 = x*b1 - b2 + f[i]/2 */

    t0 = L_shl (t0, 6);

    cheb = extract_h (t0);

    return (cheb);
}

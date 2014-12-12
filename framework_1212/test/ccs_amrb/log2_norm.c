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
#include "table.h"     /* Table for Log2() */
 
/*************************************************************************
 *
 *   FUNCTION:   Log2_norm()
 *
 *   PURPOSE:   Computes log2(L_x, exp),  where   L_x is positive and
 *              normalized, and exp is the normalisation exponent
 *              If L_x is negative or zero, the result is 0.
 *
 *   DESCRIPTION:
 *        The function Log2(L_x) is approximated by a table and linear
 *        interpolation. The following steps are used to compute Log2(L_x)
 *
 *           1- exponent = 30-norm_exponent
 *           2- i = bit25-b31 of L_x;  32<=i<=63  (because of normalization).
 *           3- a = bit10-b24
 *           4- i -=32
 *           5- fraction = table[i]<<16 - (table[i] - table[i+1]) * a * 2
 *
 *************************************************************************/
void Log2_norm (
    Word32 L_x,         /* (i) : input value (normalized)                    */
    Word16 exp,         /* (i) : norm_l (L_x)                                */
    Word16 *exponent,   /* (o) : Integer part of Log2.   (range: 0<=val<=30) */
    Word16 *fraction    /* (o) : Fractional part of Log2. (range: 0<=val<1)  */
)
{
    Word16 i, a, tmp;
    Word32 L_y;

     
    if (L_x <= (Word32) 0)
    {
        *exponent = 0;           
        *fraction = 0;           
        return;
    }

    *exponent = sub (30, exp);   

    L_x = L_shr (L_x, 9);
    i = extract_h (L_x);                /* Extract b25-b31 */
    L_x = L_shr (L_x, 1);
    a = extract_l (L_x);                /* Extract b10-b24 of fraction */
    a = a & (Word16) 0x7fff;     

    i = sub (i, 32);

    L_y = L_deposit_h (table_log2[i]);       /* table[i] << 16        */
    tmp = sub (table_log2[i], table_log2[i + 1]); /* table[i] - table[i+1] */
    L_y = L_msu (L_y, tmp, a);          /* L_y -= tmp*a*2        */

    *fraction = extract_h (L_y); 

    return;
}

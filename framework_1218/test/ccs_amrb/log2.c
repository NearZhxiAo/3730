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
#include "log2_norm.h"
/*************************************************************************
 *
 *   FUNCTION:   Log2()
 *
 *   PURPOSE:   Computes log2(L_x),  where   L_x is positive.
 *              If L_x is negative or zero, the result is 0.
 *
 *   DESCRIPTION:
 *        normalizes L_x and then calls Log2_norm().
 *
 *************************************************************************/
void Log2 (
    Word32 L_x,         /* (i) : input value                                 */
    Word16 *exponent,   /* (o) : Integer part of Log2.   (range: 0<=val<=30) */
    Word16 *fraction    /* (o) : Fractional part of Log2. (range: 0<=val<1) */
)
{
    Word16 exp;

    exp = norm_l (L_x);
    Log2_norm (L_shl (L_x, exp), exp, exponent, fraction);
}

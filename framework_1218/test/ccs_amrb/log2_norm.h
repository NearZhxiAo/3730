/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef Log2_norm_h
#define Log2_norm_h "$Id $"

#include "typedef.h"
 
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
);
#endif

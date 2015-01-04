/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef sqrt_l_exp_h
#define sqrt_l_exp_h "$Id $"
 
#include "typedef.h"
 
Word32 sqrt_l_exp (/* o : output value,                        Q31 */
    Word32 L_x,    /* i : input value,                         Q31 */
    Word16 *exp    /* o : right shift to be applied to result, Q0  */
);
 
#endif

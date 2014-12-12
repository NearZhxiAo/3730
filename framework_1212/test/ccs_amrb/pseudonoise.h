/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef pseudonoise_h
#define pseudonoise_h "$Id $"

#include "typedef.h"

/*************************************************************************
 *
 *   FUNCTION NAME: pseudonoise
 *
 *************************************************************************/
Word16 pseudonoise (
    Word32 *shift_reg, /* i/o : Old CN generator shift register state */
    Word16 no_bits     /* i   : Number of bits                        */
);
#endif

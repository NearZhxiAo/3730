/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef pow2_h
#define pow2_h "$Id $"
 
#include "typedef.h"
 
Word32 Pow2 (          /* (o) : result       (range: 0<=val<=0x7fffffff)    */
    Word16 exponent,   /* (i) : Integer part.      (range: 0<=val<=30)      */
    Word16 fraction    /* (i) : Fractional part.  (range: 0.0<=val<1.0)     */
);
 
#endif

/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef Interpol_3or6_h
#define Interpol_3or6_h "$Id $"
 
#include "typedef.h"
 
Word16 Interpol_3or6 (  /* (o)  : interpolated value                        */
    Word16 *x,          /* (i)  : input vector                              */
    Word16 frac,        /* (i)  : fraction  (-2..2 for 3*, -3..3 for 6*)    */
    Word16 flag3        /* (i)  : if set, upsampling rate = 3 (6 otherwise) */
);
 
#endif

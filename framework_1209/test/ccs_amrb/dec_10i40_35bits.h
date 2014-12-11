/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef dec_10i40_35bits_h
#define dec_10i40_35bits_h "$Id $"
 
#include "typedef.h"

void dec_10i40_35bits (
    Word16 index[],    /* (i)   : index of 10 pulses (sign+position)        */
    Word16 cod[]       /* (o)   : algebraic (fixed) codebook excitation     */
);
 
#endif

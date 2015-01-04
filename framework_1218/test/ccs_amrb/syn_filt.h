/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef syn_filt_h
#define syn_filt_h "$Id $"
#include "typedef.h"

void Syn_filt (
    Word16 a[],        /* (i)  : a[m+1] prediction coefficients   (m=10)    */
    Word16 x[],        /* (i)  : input signal                               */
    Word16 y[],        /* (o)  : output signal                              */
    Word16 lg,         /* (i)  : size of filtering                          */
    Word16 mem[],      /* (i/o): memory associated with this filtering.     */
    Word16 update      /* (i)  : 0=no update, 1=update of memory.           */
);
 
#endif

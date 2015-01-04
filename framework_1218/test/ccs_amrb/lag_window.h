/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef Lag_window_h
#define Lag_window_h "$Id $"
 
#include "typedef.h"
 
void Lag_window (
    Word16 m,          /* (i)    : LPC order                                */
    Word16 r_h[],      /* (i/o)  : Autocorrelations  (msb)                  */
    Word16 r_l[]       /* (i/o)  : Autocorrelations  (lsb)                  */
);
 
#endif

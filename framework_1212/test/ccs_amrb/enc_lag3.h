/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef enc_lag3_h
#define enc_lag3_h "$Id $"
 
#include "typedef.h"
 
Word16
Enc_lag3(                /* o  : Return index of encoding     */
    Word16 T0,           /* i  : Pitch delay                          */
    Word16 T0_frac,      /* i  : Fractional pitch delay               */
	 Word16 T0_prev,      /* i  : Integer pitch delay of last subframe */
	 Word16 T0_min,       /* i  : minimum of search range              */
	 Word16 T0_max,       /* i  : maximum of search range              */
    Word16 delta_flag,   /* i  : Flag for 1st (or 3rd) subframe       */
	 Word16 flag4         /* i  : Flag for encoding with 4 bits        */
);
 
#endif

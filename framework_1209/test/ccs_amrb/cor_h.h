/*
*****************************************************************************
*                                                                     
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*                                                                     
*****************************************************************************
*/                                                                    
#ifndef cor_h_h
#define cor_h_h "@(#)$Id $"

#include "typedef.h"                                                         
#include "cnst.h"                                                            
                                                                             
void cor_h (
    Word16 h[],     /* (i) : impulse response of weighted synthesis filter */
    Word16 sign[],      /* (i) : sign of d[n]                              */
    Word16 rr[][L_CODE] /* (o) : matrix of autocorrelation                 */
);

#endif
  

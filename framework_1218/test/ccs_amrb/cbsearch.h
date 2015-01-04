/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#ifndef cbsearch_h
#define cbsearch_h "$Id $"
 
/*
*****************************************************************************
*                         INCLUDE FILES                               
*****************************************************************************
*/                                                                    
#include "typedef.h"                                                  
#include "mode.h"                                                     
                                                                      
int cbsearch(Word16 x[],     /* i : target vector, Q0                      */
             Word16 h[],     /* i : impulse response of weighted synthesis */
                             /*     filter h[-L_subfr..-1] must be set to  */
                             /*    zero. Q12                               */
             Word16 T0,      /* i : Pitch lag                              */
             Word16 pitch_sharp, /* i : Last quantized pitch gain, Q14     */
             Word16 gain_pit,/* i : Pitch gain, Q14                        */ 
             Word16 res2[],  /* i : Long term prediction residual, Q0      */
             Word16 code[],  /* o : Innovative codebook, Q13               */
             Word16 y[],     /* o : filtered fixed codebook excitation, Q12 */
             Word16 **anap,  /* o : Signs of the pulses                    */
             enum Mode mode, /* i : coder mode                             */
             Word16 subNr)   /* i : subframe number                        */
;

#endif

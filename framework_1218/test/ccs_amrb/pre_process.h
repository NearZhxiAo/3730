/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef pre_process_h
#define pre_process_h "$Id $"
 
#include "typedef.h"
 
typedef struct {
  Word16 y2_hi;
  Word16 y2_lo;
  Word16 y1_hi;
  Word16 y1_lo;
  Word16 x0;
  Word16 x1;
} Pre_ProcessState;
 

int Pre_Process (
    Pre_ProcessState *st,
    Word16 signal[],   /* Input/output signal                               */
    Word16 lg          /* Lenght of signal                                  */
);

#endif

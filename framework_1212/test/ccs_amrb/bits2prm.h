/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/

#ifndef bits2prm_h
#define bits2prm_h "$Id $"
 
#include "typedef.h"
#include "mode.h"

/*
**************************************************************************
*
*  Function    : Bits2prm
*  Purpose     : Retrieves the vector of encoder parameters from 
*                the received serial bits in a frame.
*  Returns     : void
*
**************************************************************************
*/
void Bits2prm (
    enum Mode mode,
    Word16 bits[],   /* input : serial bits, (244 + bfi)               */
    Word16 prm[]     /* output: analysis parameters, (57+1 parameters) */
); 
 
#endif

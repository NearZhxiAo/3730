/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef d_gain_pitch_h
#define d_gain_pitch_h "$Id $"
 
#include "typedef.h"
#include "mode.h"
 
/*
**************************************************************************
*
*  Function    : d_gain_pitch
*  Purpose     : Decodes the pitch gain using the received index.
*  Description : In case of no frame erasure, the gain is obtained 
*                from the quantization table at the given index; 
*                otherwise, a downscaled past gain is used.
*  Returns     : Quantized pitch gain
*
**************************************************************************
*/
Word16 d_gain_pitch (      /* return value: gain (Q14)                */
    enum Mode mode,        /* i : AMR mode                            */
    Word16 index           /* i   : index of quantization             */
);
 
#endif

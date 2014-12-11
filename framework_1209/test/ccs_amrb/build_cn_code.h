/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef build_CN_code_h
#define build_CN_code_h "$Id $"

#include "typedef.h"

/***************************************************************************
*
*  Function    : build_CN_code
*
***************************************************************************/ 
void build_CN_code (
    Word32 *seed,         /* i/o : Old CN generator shift register state */
    Word16 cod[]          /* o   : Generated CN fixed codebook vector    */
);
#endif

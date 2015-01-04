/*
*****************************************************************************
**-------------------------------------------------------------------------**
**                                                                         **
**     GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001       **
**                               R99   Version 3.2.0                       **
**                               REL-4 Version 4.0.0                       **
**                                                                         **
**-------------------------------------------------------------------------**
*****************************************************************************
*/
#ifndef filter3_h
#define filter3_h "$Id $"

#include "typedef.h"
/****************************************************************************
 *
 *     Function     : filter3
 *     Purpose      : Third-order half-band lowpass/highpass filter pair with
 *                    decimation.
 *     Return value : 
 *
 ***************************************************************************/
void filter3(Word16 *in0,   /* i/o : input values; output low-pass part  */ 
                    Word16 *in1,   /* i/o : input values; output high-pass part */
                    Word16 *data   /* i/o : updated filter memory               */
                    );

#endif

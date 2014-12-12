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
#ifndef first_filter_stage_h
#define first_filter_stage_h "$Id $"

#include "typedef.h"

/****************************************************************************
 *
 *     Function     : first_filter_stage
 *     Purpose      : Scale input down by one bit. Calculate 5th order
 *                    half-band lowpass/highpass filter pair with
 *                    decimation.
 *
 ***************************************************************************/
void first_filter_stage(
						Word16 in[],  /* i   : input signal                  */
                        Word16 out[], /* o   : output values, every other    */
                                             /*       output is low-pass part and   */
                                             /*       other is high-pass part every */
                        Word16 data[] /* i/o : filter memory                 */
                        );

#endif

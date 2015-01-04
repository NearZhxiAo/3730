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
#ifndef level_calculation_h
#define level_calculation_h "$Id $"

#include "typedef.h"
/****************************************************************************
 *
 *     Function     : level_calculation
 *     Purpose      : Calculate signal level in a sub-band. Level is calculated
 *                    by summing absolute values of the input data.
 *     Return value : signal level
 *
 ***************************************************************************/
Word16 level_calculation(
    Word16 data[],     /* i   : signal buffer                                    */
    Word16 *sub_level, /* i   : level calculate at the end of the previous frame */
                       /* o   : level of signal calculated from the last         */
                       /*       (count2 - count1) samples                        */
    Word16 count1,     /* i   : number of samples to be counted                  */
    Word16 count2,     /* i   : number of samples to be counted                  */
    Word16 ind_m,      /* i   : step size for the index of the data buffer       */
    Word16 ind_a,      /* i   : starting index of the data buffer                */
    Word16 scale       /* i   : scaling for the level calculation                */
    );
#endif

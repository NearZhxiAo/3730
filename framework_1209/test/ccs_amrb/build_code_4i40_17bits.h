/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef build_code_4i40_17bits_h
#define build_code_4i40_17bits_h "$Id $"

#include "typedef.h"

/*************************************************************************
 *
 *  FUNCTION:  build_code_4i40_17bits()
 *
 *  PURPOSE: Builds the codeword, the filtered codeword and index of the
 *           codevector, based on the signs and positions of 4 pulses.
 *
 *************************************************************************/

Word16 build_code_4i40_17bits(
    Word16 codvec[],  /* i : position of pulses                            */
    Word16 dn_sign[], /* i : sign of pulses                                */
    Word16 cod[],     /* o : innovative code vector                        */
    Word16 h[],       /* i : impulse response of weighted synthesis filter */
    Word16 y[],       /* o : filtered innovative code                      */
    Word16 sign[]     /* o : index of 4 pulses (sign+position)             */
);
#endif

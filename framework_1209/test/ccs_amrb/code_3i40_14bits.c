/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/

#include "typedef.h"
#include "basic_op.h"
#include "inv_sqrt.h"
#include "cnst.h"
#include "cor_h.h"
#include "cor_h_x.h"
#include "set_sign.h"
#include "build_code_3i40_14bits.h"
#include "search_3i40_14bits.h"
#define NB_PULSE3  3

/*************************************************************************
 *
 *  FUNCTION:  code_3i40_14bits()
 *
 *  PURPOSE:  Searches a 14 bit algebraic codebook containing 3 pulses
 *            in a frame of 40 samples.
 *
 *  DESCRIPTION:
 *    The code length is 40, containing 3 nonzero pulses: i0...i2.
 *    All pulses can have two possible amplitudes: +1 or -1.
 *    Pulse i0 can have 8 possible positions, pulses i1 and i2 can have
 *    2x8=16 positions.
 *
 *       i0 :  0, 5, 10, 15, 20, 25, 30, 35.
 *       i1 :  1, 6, 11, 16, 21, 26, 31, 36.
 *             3, 8, 13, 18, 23, 28, 33, 38.
 *       i2 :  2, 7, 12, 17, 22, 27, 32, 37.
 *             4, 9, 14, 19, 24, 29, 34, 39.
 *
 *************************************************************************/

Word16 code_3i40_14bits(
    Word16 x[],         /* i : target vector                                 */
    Word16 h[],         /* i : impulse response of weighted synthesis filter */
                        /*     h[-L_subfr..-1] must be set to zero.          */
    Word16 T0,          /* i : Pitch lag                                     */
    Word16 pitch_sharp, /* i : Last quantized pitch gain                     */
    Word16 code[],      /* o : Innovative codebook                           */
    Word16 y[],         /* o : filtered fixed codebook excitation            */
    Word16 * sign       /* o : Signs of 3 pulses                             */
)
{
    Word16 codvec[NB_PULSE3];
    Word16 dn[L_CODE], dn2[L_CODE], dn_sign[L_CODE];
    Word16 rr[L_CODE][L_CODE];
    Word16 i, index, sharp;

    sharp = shl(pitch_sharp, 1);
    
    if (T0 < L_CODE)
    {
       for (i = T0; i < L_CODE; i++) 
	   {
          h[i] = add(h[i], mult(h[i - T0], sharp));      
       }
    }
    
    cor_h_x(h, x, dn, 1);
    set_sign(dn, dn_sign, dn2, 6);
    cor_h(h, dn_sign, rr);
    search_3i40_14bits(dn, dn2, rr, codvec);
                                     /* function result */
    index = build_code_3i40_14bits(codvec, dn_sign, code, h, y, sign);

  /*-----------------------------------------------------------------*
  * Compute innovation vector gain.                                 *
  * Include fixed-gain pitch contribution into code[].              *
  *-----------------------------------------------------------------*/

    
    if (T0 < L_CODE)
    {
       for (i = T0; i < L_CODE; i++) 
	   {
          code[i] = add(code[i], mult(code[i - T0], sharp));    
       }
    }
    return index;
}


/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"
#include "cnst.h"
#include "inv_sqrt.h"
#include "cor_h_x2.h"
#include "cor_h.h"
#include "set_sign.h"
#include "search_10and8i40.h"
#include "compress_code.h"
#include "build_code_8i40_31bits.h"
#include "set_sign12k2.h"

#define NB_PULSE8 8


/*************************************************************************
 *
 *  FUNCTION:  code_8i40_31bits()
 *
 *  PURPOSE:  Searches a 31 bit algebraic codebook containing 8 pulses
 *            in a frame of 40 samples.
 *
 *  DESCRIPTION:
 *    The code contains 8 nonzero pulses: i0...i7.
 *    All pulses can have two possible amplitudes: +1 or -1.
 *    The 40 positions in a subframe are divided into 4 tracks of
 *    interleaved positions. Each track contains two pulses.
 *    The pulses can have the following possible positions:
 *
 *       i0, i4 :  0, 4, 8,  12, 16, 20, 24, 28, 32, 36
 *       i1, i5 :  1, 5, 9,  13, 17, 21, 25, 29, 33, 37
 *       i2, i6 :  2, 6, 10, 14, 18, 22, 26, 30, 34, 38
 *       i3, i7 :  3, 7, 11, 15, 19, 23, 27, 31, 35, 39
 *
 *    Each pair of pulses require 1 bit for their signs. The positions 
 *    are encoded together 3,3 and 2 resulting in   
 *    (7+3) + (7+3) + (5+2) bits for their
 *    positions. This results in a 31 (4 sign and 27 pos) bit codebook.
 *    The function determines the optimal pulse signs and positions, builds
 *    the codevector, and computes the filtered codevector.
 *
 *************************************************************************/

void code_8i40_31bits (
   Word16 x[],        /* i : target vector                                  */
   Word16 cn[],       /* i : residual after long term prediction            */
   Word16 h[],        /* i : impulse response of weighted synthesis
                             filter                                         */
   Word16 cod[],      /* o : algebraic (fixed) codebook excitation          */
   Word16 y[],        /* o : filtered fixed codebook excitation             */
   Word16 indx[]      /* o : 7 Word16, index of 8 pulses (signs+positions)  */
)
{
   Word16 ipos[NB_PULSE8], pos_max[NB_TRACK_MR102], codvec[NB_PULSE8];
   Word16 dn[L_CODE], sign[L_CODE];
   Word16 rr[L_CODE][L_CODE];
   Word16 linear_signs[NB_TRACK_MR102];
   Word16 linear_codewords[NB_PULSE8];

   cor_h_x2 (h, x, dn, 2, NB_TRACK_MR102, STEP_MR102);
   /* 2 = use GSMEFR scaling */

   set_sign12k2 (dn, cn, sign, pos_max, NB_TRACK_MR102, ipos, STEP_MR102);
   /* same setsign alg as GSM-EFR new constants though*/
   
   cor_h (h, sign, rr);
   search_10and8i40 (NB_PULSE8, STEP_MR102, NB_TRACK_MR102,
                     dn, rr, ipos, pos_max, codvec);
   
   build_code_8i40_31bits (codvec, sign, cod, h, y, linear_signs, linear_codewords);
   compress_code (linear_signs, linear_codewords, indx);
   
   return;
}

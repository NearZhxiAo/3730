/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef d_plsf_h
#define d_plsf_h "$Id $"
 
#include "typedef.h"
#include "cnst.h"
#include "mode.h"

typedef struct {
  Word16 past_r_q[M];   /* Past quantized prediction error, Q15 */
  Word16 past_lsf_q[M]; /* Past dequantized lsfs,           Q15 */
} D_plsfState;

#endif

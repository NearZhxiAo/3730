/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef int_lpc_1to3_2_h
#define int_lpc_1to3_2_h "$Id $"

#include "typedef.h"

/*************************************************************************
 * Function Int_lpc_1to3_2()
 * Interpolation of the LPC parameters.
 * Same as the previous function but we do not recompute Az() for
 * subframe 4 because it is already available.
 *************************************************************************/

void Int_lpc_1to3_2(
     Word16 lsp_old[],  /* input : LSP vector at the 4th SF of past frame    */
     Word16 lsp_new[],  /* input : LSP vector at the 4th SF of present frame */
     Word16 Az[]        /* output: interpolated LP parameters in SFs 1,2,3   */
);
#endif


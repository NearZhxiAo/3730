/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef int_lpc_1and3_2_h
#define int_lpc_1and3_2_h "$Id $"

#include "typedef.h"

/*
**************************************************************************
*
*  Function    : Int_lpc_1and3_2
*  Purpose     : Interpolation of the LPC parameters. Same as the Int_lpc
*                function but we do not recompute Az() for subframe 2 and
*                4 because it is already available.
*  Returns     : void
*
**************************************************************************
*/
void Int_lpc_1and3_2 (
    Word16 lsp_old[],  /* i : LSP vector at the 4th subfr. of past frame (M) */
    Word16 lsp_mid[],  /* i : LSP vector at the 2nd subframe of
                             present frame (M)                                  */
    Word16 lsp_new[],  /* i : LSP vector at the 4th subframe of
                             present frame (M)                                  */
    Word16 Az[]        /* o :interpolated LP parameters
                             in subframes 1 and 3 (AZ_SIZE)                     */
);

#endif


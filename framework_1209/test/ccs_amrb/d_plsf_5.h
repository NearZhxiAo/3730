/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#ifndef d_plsf_5_h
#define d_plsf_5_h "$Id $"

#include "typedef.h"
#include "d_plsf.h"
/*
**************************************************************************
*
*  Function    : D_plsf_5
*  Purpose     : Decodes the 2 sets of LSP parameters in a frame 
*                using the received quantization indices.
*
**************************************************************************
*/
int D_plsf_5 (
    D_plsfState *st,    /* i/o: State variables                            */
    Word16 bfi,         /* i  : bad frame indicator (set to 1 if a bad    
                                frame is received)                         */  
    Word16 *indice,     /* i  : quantization indices of 5 submatrices, Q0  */
    Word16 *lsp1_q,     /* o  : quantized 1st LSP vector (M),          Q15 */
    Word16 *lsp2_q      /* o  : quantized 2nd LSP vector (M),          Q15 */
);
#endif


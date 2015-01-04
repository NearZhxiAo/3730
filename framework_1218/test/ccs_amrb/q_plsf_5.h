/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef Q_plsf_5_h
#define Q_plsf_5_h "$Id $"

#include "typedef.h"
#include "q_plsf.h"
/*************************************************************************
 *   FUNCTION:  Q_plsf_5()
 *
 *   PURPOSE:  Quantization of 2 sets of LSF parameters using 1st order MA
 *             prediction and split by 5 matrix quantization (split-MQ)
 *
 *   DESCRIPTION:
 *
 *        p[i] = pred_factor*past_rq[i];   i=0,...,m-1
 *        r1[i]= lsf1[i] - p[i];           i=0,...,m-1
 *        r2[i]= lsf2[i] - p[i];           i=0,...,m-1
 *   where:
 *        lsf1[i]           1st mean-removed LSF vector.
 *        lsf2[i]           2nd mean-removed LSF vector.
 *        r1[i]             1st residual prediction vector.
 *        r2[i]             2nd residual prediction vector.
 *        past_r2q[i]       Past quantized residual (2nd vector).
 *
 *   The residual vectors r1[i] and r2[i] are jointly quantized using
 *   split-MQ with 5 codebooks. Each 4th dimension submatrix contains 2
 *   elements from each residual vector. The 5 submatrices are as follows:
 *     {r1[0], r1[1], r2[0], r2[1]};  {r1[2], r1[3], r2[2], r2[3]};
 *     {r1[4], r1[5], r2[4], r2[5]};  {r1[6], r1[7], r2[6], r2[7]};
 *                    {r1[8], r1[9], r2[8], r2[9]};
 *
 *************************************************************************/
void Q_plsf_5 (
    Q_plsfState *st,
    Word16 *lsp1,      /* i : 1st LSP vector,                     Q15 */
    Word16 *lsp2,      /* i : 2nd LSP vector,                     Q15 */   
    Word16 *lsp1_q,    /* o : quantized 1st LSP vector,           Q15 */
    Word16 *lsp2_q,    /* o : quantized 2nd LSP vector,           Q15 */
    Word16 *indice     /* o : quantization indices of 5 matrices, Q0  */
);
#endif

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
#include "lsp_lsf.h"
#include "lsf_lsp.h"
#include "reorder_lsf.h"
#include "q_plsf.h"
#include "table.h"         /* Codebooks of LSF prediction residual */

#include "lsf_wt.h"
#include "vq_subvec.h"
#include "vq_subvec_s.h"
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
)
{
    Word16 i;
    Word16 lsf1[M], lsf2[M], wf1[M], wf2[M], lsf_p[M], lsf_r1[M], lsf_r2[M];
    Word16 lsf1_q[M], lsf2_q[M];

    /* convert LSFs to normalize frequency domain 0..16384  */

    Lsp_lsf (lsp1, lsf1, M);
    Lsp_lsf (lsp2, lsf2, M);

    /* Compute LSF weighting factors (Q13) */
    
    Lsf_wt (lsf1, wf1);
    Lsf_wt (lsf2, wf2);

    /* Compute predicted LSF and prediction error */

    for (i = 0; i < M; i++)
    {
        lsf_p[i] = add (mean_lsf_5[i], mult (st->past_rq[i], LSP_PRED_FAC_MR122));
         
        lsf_r1[i] = sub (lsf1[i], lsf_p[i]);            
        lsf_r2[i] = sub (lsf2[i], lsf_p[i]);            
    }

    /*---- Split-MQ of prediction error ----*/

    indice[0] = Vq_subvec (&lsf_r1[0], &lsf_r2[0], dico1_lsf_5,
                           &wf1[0], &wf2[0], DICO1_SIZE_5);
                                                         

    indice[1] = Vq_subvec (&lsf_r1[2], &lsf_r2[2], dico2_lsf_5,
                           &wf1[2], &wf2[2], DICO2_SIZE_5);
                                                         

    indice[2] = Vq_subvec_s (&lsf_r1[4], &lsf_r2[4], dico3_lsf_5,
                             &wf1[4], &wf2[4], DICO3_SIZE_5);
                                                         

    indice[3] = Vq_subvec (&lsf_r1[6], &lsf_r2[6], dico4_lsf_5,
                           &wf1[6], &wf2[6], DICO4_SIZE_5);
                                                         

    indice[4] = Vq_subvec (&lsf_r1[8], &lsf_r2[8], dico5_lsf_5,
                           &wf1[8], &wf2[8], DICO5_SIZE_5);
                                                         

    /* Compute quantized LSFs and update the past quantized residual */
    for (i = 0; i < M; i++)
    {
        lsf1_q[i] = add (lsf_r1[i], lsf_p[i]);           
        lsf2_q[i] = add (lsf_r2[i], lsf_p[i]);           
        st->past_rq[i] = lsf_r2[i];                      
    }

    /* verification that LSFs has minimum distance of LSF_GAP */

    Reorder_lsf (lsf1_q, LSF_GAP, M);
    Reorder_lsf (lsf2_q, LSF_GAP, M);

    /*  convert LSFs to the cosine domain */
    
    Lsf_lsp (lsf1_q, lsp1_q, M);
    Lsf_lsp (lsf2_q, lsp2_q, M);
}

/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"
#include "lsp_lsf.h"
#include "lsf_lsp.h"
#include "mode.h"
#include "reorder_lsf.h"
#include "copy.h"
#include "q_plsf.h"
#include "table.h"        /* Codebooks of LSF prediction residual */

#include "lsf_wt.h"
#include "vq_subvec3.h"
#include "vq_subvec4.h"

#define PAST_RQ_INIT_SIZE 8

/***********************************************************************
 *
 * routine:   Q_plsf_3()
 *
 * Quantization of LSF parameters with 1st order MA prediction and
 * split by 3 vector quantization (split-VQ)
 *
 ***********************************************************************/
void Q_plsf_3(
    Q_plsfState *st,    /* i/o: state struct                             */
    enum Mode mode,     /* i  : coder mode                               */
    Word16 *lsp1,       /* i  : 1st LSP vector                      Q15  */
    Word16 *lsp1_q,     /* o  : quantized 1st LSP vector            Q15  */
    Word16 *indice,     /* o  : quantization indices of 3 vectors   Q0   */
    Word16 *pred_init_i /* o  : init index for MA prediction in DTX mode */
)
{
    Word16 i, j;
    Word16 lsf1[M], wf1[M], lsf_p[M], lsf_r1[M];
    Word16 lsf1_q[M];
    
    Word32 L_pred_init_err;
    Word32 L_min_pred_init_err;
    Word16 temp_r1[M];
    Word16 temp_p[M];

    /* convert LSFs to normalize frequency domain 0..16384 */

    Lsp_lsf(lsp1, lsf1, M);

    /* compute LSF weighting factors (Q13) */

    Lsf_wt(lsf1, wf1);

    /* Compute predicted LSF and prediction error */
    if (mode != MRDTX)
    {
       for (i = 0; i < M; i++)
       {
          lsf_p[i] = add(mean_lsf_3[i], 
                         mult(st->past_rq[i], 
                              pred_fac[i]));                            
          lsf_r1[i] = sub(lsf1[i], lsf_p[i]);                           
      }
    }
    else
    {
       /* DTX mode, search the init vector that yields */
       /* lowest prediction resuidual energy           */
       *pred_init_i = 0;                                                
       L_min_pred_init_err = 0x7fffffff; /* 2^31 - 1 */                 
       for (j = 0; j < PAST_RQ_INIT_SIZE; j++)
       {
          L_pred_init_err = 0;                                          
          for (i = 0; i < M; i++)
          {
             temp_p[i] = add(mean_lsf_3[i], past_rq_init[j*M+i]);
             temp_r1[i] = sub(lsf1[i],temp_p[i]);
             L_pred_init_err = L_mac(L_pred_init_err, temp_r1[i], temp_r1[i]);
          }  /* next i */

          
          if (L_pred_init_err < L_min_pred_init_err)
          {
             L_min_pred_init_err = L_pred_init_err;                      
             Copy(temp_r1, lsf_r1, M);
             Copy(temp_p, lsf_p, M);
             
             /* Set zerom */
             Copy(&past_rq_init[j*M], st->past_rq, M);
             *pred_init_i = j;                                                      
          } /* endif */
       } /* next j */
    } /* endif MRDTX */
    
    /*---- Split-VQ of prediction error ----*/
    if ((mode == MR475) || (mode == MR515))
    {   /* MR475, MR515 */
        
                          
      indice[0] = Vq_subvec3(&lsf_r1[0], dico1_lsf_3, &wf1[0], DICO1_SIZE_3, 0);
      
      indice[1] = Vq_subvec3(&lsf_r1[3], dico2_lsf_3, &wf1[3], DICO2_SIZE_3/2, 1);
      
      indice[2] = Vq_subvec4(&lsf_r1[6], mr515_3_lsf, &wf1[6], MR515_3_SIZE);
      
    }
    else if (mode == MR795)
    {   /* MR795 */
        
       
      indice[0] = Vq_subvec3(&lsf_r1[0], mr795_1_lsf, &wf1[0], MR795_1_SIZE, 0);
      
      indice[1] = Vq_subvec3(&lsf_r1[3], dico2_lsf_3, &wf1[3], DICO2_SIZE_3, 0);
      
      indice[2] = Vq_subvec4(&lsf_r1[6], dico3_lsf_3, &wf1[6], DICO3_SIZE_3);
      
    }
    else 
    {   /* MR59, MR67, MR74, MR102 , MRDTX */
        

      indice[0] = Vq_subvec3(&lsf_r1[0], dico1_lsf_3, &wf1[0], DICO1_SIZE_3, 0);
      
      indice[1] = Vq_subvec3(&lsf_r1[3], dico2_lsf_3, &wf1[3], DICO2_SIZE_3, 0);
      
      indice[2] = Vq_subvec4(&lsf_r1[6], dico3_lsf_3, &wf1[6], DICO3_SIZE_3);
      
    }

        
    /* Compute quantized LSFs and update the past quantized residual */

    for (i = 0; i < M; i++)
    {
        lsf1_q[i] = add(lsf_r1[i], lsf_p[i]);                        
        st->past_rq[i] = lsf_r1[i];                                  
    }

    /* verification that LSFs has mimimum distance of LSF_GAP Hz */

    Reorder_lsf(lsf1_q, LSF_GAP, M);

    /*  convert LSFs to the cosine domain */

    Lsf_lsp(lsf1_q, lsp1_q, M);
}

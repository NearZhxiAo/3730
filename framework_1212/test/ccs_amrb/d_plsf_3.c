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
#include "reorder_lsf.h"
#include "copy.h"
#include "lsf_lsp.h"
#include "table.h"    /* Codebooks of LSF prediction residual */
#include "d_plsf.h"

/* ALPHA    ->  0.9                                            */
/* ONE_ALPHA-> (1.0-ALPHA)                                     */

#define ALPHA     29491
#define ONE_ALPHA 3277

/*************************************************************************
 *
 *  FUNCTION:   D_plsf_3()
 *
 *  PURPOSE: Decodes the LSP parameters using the received quantization
 *           indices.1st order MA prediction and split by 3 vector
 *           quantization (split-VQ)
 *
 *************************************************************************/

void D_plsf_3(
    D_plsfState *st,   /* i/o: State struct                               */
    enum Mode mode,    /* i  : coder mode                                 */
    Word16 bfi,        /* i  : bad frame indicator (set to 1 if a         */
                       /*      bad frame is received)                     */
    Word16 * indice,   /* i  : quantization indices of 3 submatrices, Q0  */
    Word16 * lsp1_q    /* o  : quantized 1st LSP vector,              Q15 */
)
{
    Word16 i, index;
    const Word16 *p_cb1, *p_cb2, *p_cb3, *p_dico;
	Word16 temp;
    Word16 lsf1_r[M];
    Word16 lsf1_q[M];
    
    
    if (bfi != 0)   /* if bad frame */
    {
        /* use the past LSFs slightly shifted towards their mean */

        for (i = 0; i < M; i++)
        {
            /* lsfi_q[i] = ALPHA*past_lsf_q[i] + ONE_ALPHA*mean_lsf[i]; */

            lsf1_q[i] = add(mult(st->past_lsf_q[i], ALPHA),
                            mult(mean_lsf_3[i], ONE_ALPHA));
                                                
        }

        /* estimate past quantized residual to be used in next frame */
	
	if (sub(mode, MRDTX) != 0) {
	  for (i = 0; i < M; i++) {
            /* temp  = mean_lsf[i] +  past_r2_q[i] * PRED_FAC; */
	    
            temp = add(mean_lsf_3[i], mult(st->past_r_q[i], pred_fac[i]));
	    
            st->past_r_q[i] = sub(lsf1_q[i], temp);                   
	  }
	} else {
	  for (i = 0; i < M; i++) {
            /* temp  = mean_lsf[i] +  past_r2_q[i]; */
	    
            temp = add(mean_lsf_3[i], st->past_r_q[i]);
            st->past_r_q[i] = sub(lsf1_q[i], temp);                   
	  }	  
	}
    }
    else  /* if good LSFs received */
    {
        
       if (sub (mode, MR475) == 0 || sub (mode, MR515) == 0)
       {   /* MR475, MR515 */
          p_cb1 = dico1_lsf_3;                  
          p_cb2 = dico2_lsf_3;                  
          p_cb3 = mr515_3_lsf;                
       }
       else if (sub (mode, MR795) == 0)
       {   /* MR795 */
          
          p_cb1 = mr795_1_lsf;                
          p_cb2 = dico2_lsf_3;                  
          p_cb3 = dico3_lsf_3;                  
       }
       else 
       {   /* MR59, MR67, MR74, MR102, MRDTX */
                    
          p_cb1 = dico1_lsf_3;                  
          p_cb2 = dico2_lsf_3;                  
          p_cb3 = dico3_lsf_3;                  
       }
       
       /* decode prediction residuals from 3 received indices */

        index = *indice++;                      
        p_dico = &p_cb1[add(index, add(index, index))];               
        lsf1_r[0] = *p_dico++;                  
        lsf1_r[1] = *p_dico++;                  
        lsf1_r[2] = *p_dico++;                  

        index = *indice++;                      
        
         
        if ((sub (mode, MR475) == 0) || (sub (mode, MR515) == 0))
        {   /* MR475, MR515 only using every second entry */
            index = shl(index,1);
        }
        
        p_dico = &p_cb2[add(index, add(index, index))];               
        lsf1_r[3] = *p_dico++;                  
        lsf1_r[4] = *p_dico++;                  
        lsf1_r[5] = *p_dico++;                  

        index = *indice++;                      
        p_dico = &p_cb3[shl(index, 2)];         
        lsf1_r[6] = *p_dico++;                  
        lsf1_r[7] = *p_dico++;                  
        lsf1_r[8] = *p_dico++;                  
        lsf1_r[9] = *p_dico++;                  

        /* Compute quantized LSFs and update the past quantized residual */

	if (sub(mode, MRDTX) != 0) 
           for (i = 0; i < M; i++) {
              temp = add(mean_lsf_3[i], mult(st->past_r_q[i], pred_fac[i]));
              lsf1_q[i] = add(lsf1_r[i], temp);   
              st->past_r_q[i] = lsf1_r[i];        
           }
        else
           for (i = 0; i < M; i++) {
              temp = add(mean_lsf_3[i], st->past_r_q[i]);
              lsf1_q[i] = add(lsf1_r[i], temp);   
              st->past_r_q[i] = lsf1_r[i];        
           }
    }

    /* verification that LSFs has minimum distance of LSF_GAP Hz */

    Reorder_lsf(lsf1_q, LSF_GAP, M);

    Copy (lsf1_q, st->past_lsf_q, M);

    /*  convert LSFs to the cosine domain */

    Lsf_lsp(lsf1_q, lsp1_q, M);

    return;
}


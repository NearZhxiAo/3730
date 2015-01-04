/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#include "dtx_def.h"
#include "q_plsf.h"
#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "copy.h"
#include "set_zero.h"
#include "mode.h"
#include "log2.h"
#include "lsp_lsf.h"
#include "reorder_lsf.h"
#include "table.h"
#include "lsf_lsp.h"
#include "lsf_lsp.h"
#include "q_plsf_3.h"
/*
**************************************************************************
*
*  Function    : dtx_enc
*                
**************************************************************************
*/
int dtx_enc(dtx_encState *st,        /* i/o : State struct                    */
            Word16 computeSidFlag,   /* i   : compute SID                     */
            Q_plsfState *qSt,        /* i/o : Qunatizer state struct          */
            gc_predState* predState, /* i/o : State struct                    */
	    Word16 **anap            /* o   : analysis parameters             */
	    )
{
   Word16 i,j;
   Word16 log_en;
   Word16 lsf[M];
   Word16 lsp[M];
   Word16 lsp_q[M];
   Word32 L_lsp[M];

   /* VOX mode computation of SID parameters */
    
   if ((computeSidFlag != 0)  || 
        (st->log_en_index == 0))
   {
      /* compute new SID frame if safe i.e don't
       * compute immediately after a talk spurt  */
      log_en = 0;                                           
      for (i = 0; i < M; i++)
      {
         L_lsp[i] = 0;                                      
      }
      
      /* average energy and lsp */
      for (i = 0; i < DTX_HIST_SIZE; i++)
      {
         log_en = add(log_en,
                      shr(st->log_en_hist[i],2));

         for (j = 0; j < M; j++)
         {
            L_lsp[j] = L_add(L_lsp[j],
                             L_deposit_l(st->lsp_hist[i * M + j]));
         }
      }

      log_en = shr(log_en, 1);
      for (j = 0; j < M; j++)
      {
         lsp[j] = extract_l(L_shr(L_lsp[j], 3));   /* divide by 8 */
      }

      /*  quantize logarithmic energy to 6 bits */
      st->log_en_index = add(log_en, 2560);          /* +2.5 in Q10      */
      st->log_en_index = add(st->log_en_index, 128); /* add 0.5/4 in Q10 */
      st->log_en_index = shr(st->log_en_index, 8);

      
      if (sub(st->log_en_index, 63) > 0)
      {
         st->log_en_index = 63;                             
      }
      
      if (st->log_en_index < 0)
      {
         st->log_en_index = 0;                              
      }
         
      /* update gain predictor memory */
      log_en = shl(st->log_en_index, -2+10); /* Q11 and divide by 4 */
      log_en = sub(log_en, 2560);            /* add 2.5 in Q11      */
      
      log_en = sub(log_en, 9000);
      
      if (log_en > 0)
      {
         log_en = 0;                                        
      }
      
      if (sub(log_en, -14436) < 0)
      {
         log_en = -14436;                                   
      }
      
      /* past_qua_en for other modes than MR122 */      
      predState->past_qua_en[0] = log_en;                   
      predState->past_qua_en[1] = log_en;                   
      predState->past_qua_en[2] = log_en;                   
      predState->past_qua_en[3] = log_en;                   

      /* scale down by factor 20*log10(2) in Q15 */
      log_en = mult(5443, log_en);
      
      /* past_qua_en for mode MR122 */      
      predState->past_qua_en_MR122[0] = log_en;             
      predState->past_qua_en_MR122[1] = log_en;             
      predState->past_qua_en_MR122[2] = log_en;             
      predState->past_qua_en_MR122[3] = log_en;             
 
      /* make sure that LSP's are ordered */
      Lsp_lsf(lsp, lsf, M);
      Reorder_lsf(lsf, LSF_GAP, M);
      Lsf_lsp(lsf, lsp, M);
      
      /* Quantize lsp and put on parameter list */
      Q_plsf_3(qSt, MRDTX, lsp, lsp_q, st->lsp_index, 
               &st->init_lsf_vq_index);
   }
   
   *(*anap)++ = st->init_lsf_vq_index; /* 3 bits */         
   
   *(*anap)++ = st->lsp_index[0];      /* 8 bits */         
   *(*anap)++ = st->lsp_index[1];      /* 9 bits */         
   *(*anap)++ = st->lsp_index[2];      /* 9 bits */         
   
   
   *(*anap)++ = st->log_en_index;      /* 6 bits    */      
                                       /* = 35 bits */
   
   return 0;
}


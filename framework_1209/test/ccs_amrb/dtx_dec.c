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
#include "oper_32b.h"
#include "copy.h"
#include "set_zero.h"
#include "mode.h"
#include "log2.h"
#include "lsp_az.h"
#include "pow2.h"
#include "a_refl.h"
#include "syn_filt.h"
#include "lsp_lsf.h"
#include "reorder_lsf.h"
#include "table.h"
#include "table.h"
#include "lsf_lsp.h"
#include "dtx_dec.h"
#include "Init_D_plsf_3.h"
#include "d_plsf_3.h"
#include "build_cn_code.h"
#include "pseudonoise.h"
/*
**************************************************************************
*
*  Function    : dtx_dec
*                
**************************************************************************
*/
int dtx_dec(
   dtx_decState *st,                /* i/o : State struct                    */
   Word16 mem_syn[],                /* i/o : AMR decoder state               */
   D_plsfState* lsfState,           /* i/o : decoder lsf states              */
   gc_predState* predState,         /* i/o : prediction states               */
   Cb_gain_averageState* averState, /* i/o : CB gain average states          */
   enum DTXStateType new_state,     /* i   : new DTX state                   */
   enum Mode mode,                  /* i   : AMR mode                        */
   Word16 parm[],                   /* i   : Vector of synthesis parameters  */
   Word16 synth[],                  /* o   : synthesised speech              */
   Word16 A_t[]                     /* o   : decoded LP filter in 4 subframes*/
   )
{
   Word16 log_en_index;
   Word16 i, j;
   Word16 int_fac;
   Word32 L_log_en_int;
   Word16 lsp_int[M];
   Word16 log_en_int_e;
   Word16 log_en_int_m;
   Word16 level;
   Word16 acoeff[M + 1];
   Word16 refl[M];
   Word16 pred_err;
   Word16 ex[L_SUBFR];
   Word16 ma_pred_init;
   Word16 log_pg_e, log_pg_m;
   Word16 log_pg;
   Flag negative;
   Word16 lsf_mean;
   Word32 L_lsf_mean;
   Word16 lsf_variab_index;
   Word16 lsf_variab_factor;
   Word16 lsf_int[M];
   Word16 lsf_int_variab[M];
   Word16 lsp_int_variab[M];
   Word16 acoeff_variab[M + 1];

   Word16 lsf[M];
   Word32 L_lsf[M];
   Word16 ptr;
   Word16 tmp_int_length;


   /*  This function is called if synthesis state is not SPEECH 
    *  the globally passed  inputs to this function are 
    * st->sid_frame 
    * st->valid_data 
    * st->dtxHangoverAdded
    * new_state  (SPEECH, DTX, DTX_MUTE)
    */

    
   if ((st->dtxHangoverAdded != 0) && 
       (st->sid_frame != 0))
   {
      /* sid_first after dtx hangover period */
      /* or sid_upd after dtxhangover        */

      /* set log_en_adjust to correct value */
      st->log_en_adjust = dtx_log_en_adjust[mode];
          
      ptr = add(st->lsf_hist_ptr, M);                                
      
      if (sub(ptr, 80) == 0)
      {
         ptr = 0;                                                   
      }
      Copy( &st->lsf_hist[st->lsf_hist_ptr],&st->lsf_hist[ptr],M); 
      
      ptr = add(st->log_en_hist_ptr,1);                             
      
      if (sub(ptr, DTX_HIST_SIZE) == 0)
      {
         ptr = 0;                                                   
      }
      
      st->log_en_hist[ptr] = st->log_en_hist[st->log_en_hist_ptr]; /* Q11 */
      
      /* compute mean log energy and lsp *
       * from decoded signal (SID_FIRST) */         
      st->log_en = 0;                                               
      for (i = 0; i < M; i++)
      {
         L_lsf[i] = 0;                                              
      }
      
      /* average energy and lsp */
      for (i = 0; i < DTX_HIST_SIZE; i++)
      {
         st->log_en = add(st->log_en,
                          shr(st->log_en_hist[i],3));
         for (j = 0; j < M; j++)
         {
            L_lsf[j] = L_add(L_lsf[j],
                             L_deposit_l(st->lsf_hist[i * M + j]));
         }
      }
       
      for (j = 0; j < M; j++)
      {
         lsf[j] = extract_l(L_shr(L_lsf[j],3)); /* divide by 8 */  
      }
      
      Lsf_lsp(lsf, st->lsp, M); 

      /* make log_en speech coder mode independent */
      /* added again later before synthesis        */
      st->log_en = sub(st->log_en, st->log_en_adjust);

      /* compute lsf variability vector */
      Copy(st->lsf_hist, st->lsf_hist_mean, 80);

      for (i = 0; i < M; i++)
      {
         L_lsf_mean = 0;                                           
         /* compute mean lsf */
         for (j = 0; j < 8; j++)
         {
            L_lsf_mean = L_add(L_lsf_mean, 
                               L_deposit_l(st->lsf_hist_mean[i+j*M]));
         }
         
         lsf_mean = extract_l(L_shr(L_lsf_mean, 3));               
         /* subtract mean and limit to within reasonable limits  *
          * moreover the upper lsf's are attenuated              */
         for (j = 0; j < 8; j++)
         {
            /* subtract mean */ 
            st->lsf_hist_mean[i+j*M] = 
               sub(st->lsf_hist_mean[i+j*M], lsf_mean);

            /* attenuate deviation from mean, especially for upper lsf's */
            st->lsf_hist_mean[i+j*M] = 
               mult(st->lsf_hist_mean[i+j*M], lsf_hist_mean_scale[i]);

            /* limit the deviation */
            
            if (st->lsf_hist_mean[i+j*M] < 0)
            {
               negative = 1;                                        
            }
            else
            {
               negative = 0;                                        
            }
            st->lsf_hist_mean[i+j*M] = abs_s(st->lsf_hist_mean[i+j*M]);

            /* apply soft limit */
            
            if (sub(st->lsf_hist_mean[i+j*M], 655) > 0)
            {
               st->lsf_hist_mean[i+j*M] = 
                  add(655, shr(sub(st->lsf_hist_mean[i+j*M], 655), 2));
            }
            
            /* apply hard limit */
            
            if (sub(st->lsf_hist_mean[i+j*M], 1310) > 0)
            {
               st->lsf_hist_mean[i+j*M] = 1310;                     
            }
            
            if (negative != 0) 
            {
               st->lsf_hist_mean[i+j*M] = -st->lsf_hist_mean[i+j*M];
            }
            
         }
      }
   }
   
   
   if (st->sid_frame != 0 )
   {
      /* Set old SID parameters, always shift */
      /* even if there is no new valid_data   */
      Copy(st->lsp, st->lsp_old, M);
      st->old_log_en = st->log_en;                                  
      
      
      if (st->valid_data != 0 )  /* new data available (no CRC) */
      {         
         /* Compute interpolation factor, since the division only works *
          * for values of since_last_sid < 32 we have to limit the      *
          * interpolation to 32 frames                                  */
         tmp_int_length = st->since_last_sid;                       
         st->since_last_sid = 0;                                    

         
         if (sub(tmp_int_length, 32) > 0)
         {
            tmp_int_length = 32;                                    
         }
         
         if (sub(tmp_int_length, 2) >= 0)
         {
            
            st->true_sid_period_inv = div_s(1 << 10, shl(tmp_int_length, 10)); 
         }
         else
         {
            st->true_sid_period_inv = 1 << 14; /* 0.5 it Q15 */     
         }
         
         Init_D_plsf_3(lsfState, parm[0]);  /* temporay initialization */ 
         D_plsf_3(lsfState, MRDTX, 0, &parm[1], st->lsp);
         Set_zero(lsfState->past_r_q, M);   /* reset for next speech frame */ 

         log_en_index = parm[4];                                    
         /* Q11 and divide by 4 */
         st->log_en = shl(log_en_index, (11 - 2));                  
         
         /* Subtract 2.5 in Q11 */
         st->log_en = sub(st->log_en, (2560 * 2));
         
         /* Index 0 is reserved for silence */
         
         if (log_en_index == 0)
         {
            st->log_en = MIN_16;                                    
         }
         
         /* no interpolation at startup after coder reset        */
         /* or when SID_UPD has been received right after SPEECH */
          
         if ((st->data_updated == 0) ||
             (sub(st->dtxGlobalState, SPEECH) == 0)
             ) 
         {
            Copy(st->lsp, st->lsp_old, M);
            st->old_log_en = st->log_en;                            
         }         
      } /* endif valid_data */

      /* initialize gain predictor memory of other modes */       
      ma_pred_init = sub(shr(st->log_en,1), 9000);                  
      
      if (ma_pred_init > 0)
      {                   
         ma_pred_init = 0;                                            
      }      
      
      if (sub(ma_pred_init, -14436) < 0)
      {
         ma_pred_init = -14436;                                     
      }
      
      predState->past_qua_en[0] = ma_pred_init;                     
      predState->past_qua_en[1] = ma_pred_init;                     
      predState->past_qua_en[2] = ma_pred_init;                     
      predState->past_qua_en[3] = ma_pred_init;                     

      /* past_qua_en for other modes than MR122 */      
      ma_pred_init = mult(5443, ma_pred_init); 
      /* scale down by factor 20*log10(2) in Q15 */
      predState->past_qua_en_MR122[0] = ma_pred_init;               
      predState->past_qua_en_MR122[1] = ma_pred_init;               
      predState->past_qua_en_MR122[2] = ma_pred_init;               
      predState->past_qua_en_MR122[3] = ma_pred_init;               
   } /* endif sid_frame */
   
   /* CN generation */
   /* recompute level adjustment factor Q11             *
    * st->log_en_adjust = 0.9*st->log_en_adjust +       *
    *                     0.1*dtx_log_en_adjust[mode]); */
   
   st->log_en_adjust = add(mult(st->log_en_adjust, 29491),
                           shr(mult(shl(dtx_log_en_adjust[mode],5),3277),5));

   /* Interpolate SID info */
   int_fac = shl(add(1,st->since_last_sid), 10); /* Q10 */          
   int_fac = mult(int_fac, st->true_sid_period_inv); /* Q10 * Q15 -> Q10 */
   
   /* Maximize to 1.0 in Q10 */
   
   if (sub(int_fac, 1024) > 0)
   {
      int_fac = 1024;                                               
   }
   int_fac = shl(int_fac, 4); /* Q10 -> Q14 */
   
   L_log_en_int = L_mult(int_fac, st->log_en); /* Q14 * Q11->Q26 */ 
   for(i = 0; i < M; i++)
   {
      lsp_int[i] = mult(int_fac, st->lsp[i]);/* Q14 * Q15 -> Q14 */ 
   }
   
   int_fac = sub(16384, int_fac); /* 1-k in Q14 */                  

   /* (Q14 * Q11 -> Q26) + Q26 -> Q26 */
   L_log_en_int = L_mac(L_log_en_int, int_fac, st->old_log_en);
   for(i = 0; i < M; i++)
   {
      /* Q14 + (Q14 * Q15 -> Q14) -> Q14 */
      lsp_int[i] = add(lsp_int[i], mult(int_fac, st->lsp_old[i]));  
      lsp_int[i] = shl(lsp_int[i], 1); /* Q14 -> Q15 */             
   }
   
   /* compute the amount of lsf variability */
   lsf_variab_factor = sub(st->log_pg_mean,2457); /* -0.6 in Q12 */ 
   /* *0.3 Q12*Q15 -> Q12 */
   lsf_variab_factor = sub(4096, mult(lsf_variab_factor, 9830)); 

   /* limit to values between 0..1 in Q12 */ 
   
   if (sub(lsf_variab_factor, 4096) > 0)
   {
      lsf_variab_factor = 4096;                                     
   }
   
   if (lsf_variab_factor < 0)
   {
      lsf_variab_factor = 0;                                         
   }
   lsf_variab_factor = shl(lsf_variab_factor, 3); /* -> Q15 */      

   /* get index of vector to do variability with */
   lsf_variab_index = pseudonoise(&st->L_pn_seed_rx, 3);            

   /* convert to lsf */
   Lsp_lsf(lsp_int, lsf_int, M);

   /* apply lsf variability */
   Copy(lsf_int, lsf_int_variab, M);
   for(i = 0; i < M; i++)
   {
      
      lsf_int_variab[i] = add(lsf_int_variab[i], 
                              mult(lsf_variab_factor,
                                   st->lsf_hist_mean[i+lsf_variab_index*M]));
   }

   /* make sure that LSP's are ordered */
   Reorder_lsf(lsf_int, LSF_GAP, M);
   Reorder_lsf(lsf_int_variab, LSF_GAP, M);

   /* copy lsf to speech decoders lsf state */
   Copy(lsf_int, lsfState->past_lsf_q, M);

   /* convert to lsp */
   Lsf_lsp(lsf_int, lsp_int, M);
   Lsf_lsp(lsf_int_variab, lsp_int_variab, M);

   /* Compute acoeffs Q12 acoeff is used for level    * 
    * normalization and postfilter, acoeff_variab is  *
    * used for synthesis filter                       *
    * by doing this we make sure that the level       *
    * in high frequenncies does not jump up and down  */

   Lsp_Az(lsp_int, acoeff);
   Lsp_Az(lsp_int_variab, acoeff_variab);
   
   /* For use in postfilter */
   Copy(acoeff, &A_t[0],           M + 1);
   Copy(acoeff, &A_t[M + 1],       M + 1);
   Copy(acoeff, &A_t[2 * (M + 1)], M + 1);
   Copy(acoeff, &A_t[3 * (M + 1)], M + 1);
   
   /* Compute reflection coefficients Q15 */
   A_Refl(&acoeff[1], refl);
   
   /* Compute prediction error in Q15 */
   pred_err = MAX_16; /* 0.99997 in Q15 */                          
   for (i = 0; i < M; i++)
   { 
      pred_err = mult(pred_err, sub(MAX_16, mult(refl[i], refl[i])));
   }

   /* compute logarithm of prediction gain */   
   Log2(L_deposit_l(pred_err), &log_pg_e, &log_pg_m);
   
   /* convert exponent and mantissa to Word16 Q12 */
   log_pg = shl(sub(log_pg_e,15), 12);  /* Q12 */                   
   log_pg = shr(sub(0,add(log_pg, shr(log_pg_m, 15-12))), 1);       
   st->log_pg_mean = add(mult(29491,st->log_pg_mean),
                         mult(3277, log_pg));                       

   /* Compute interpolated log energy */
   L_log_en_int = L_shr(L_log_en_int, 10); /* Q26 -> Q16 */         

   /* Add 4 in Q16 */
   L_log_en_int = L_add(L_log_en_int, 4 * 65536L);                  

   /* subtract prediction gain */
   L_log_en_int = L_sub(L_log_en_int, L_shl(L_deposit_l(log_pg), 4));

   /* adjust level to speech coder mode */
   L_log_en_int = L_add(L_log_en_int, 
                        L_shl(L_deposit_l(st->log_en_adjust), 5));  
       
   log_en_int_e = extract_h(L_log_en_int);                    
   
   log_en_int_m = extract_l(L_shr(L_sub(L_log_en_int, 
                                        L_deposit_h(log_en_int_e)), 1));
   level = extract_l(Pow2(log_en_int_e, log_en_int_m)); /* Q4 */ 
   
   for (i = 0; i < 4; i++)
   {             
      /* Compute innovation vector */
      build_CN_code(&st->L_pn_seed_rx, ex);
      for (j = 0; j < L_SUBFR; j++)
      {
         ex[j] = mult(level, ex[j]);                                
      }
      /* Synthesize */
      Syn_filt(acoeff_variab, ex, &synth[i * L_SUBFR], L_SUBFR, 
               mem_syn, 1);
      
   } /* next i */
   
   /* reset codebook averaging variables */ 
   averState->hangVar = 20;                                         
   averState->hangCount = 0;                                        
    
   
   if (sub(new_state, DTX_MUTE) == 0)
   {
      /* mute comfort noise as it has been quite a long time since  
       * last SID update  was performed                            */
      
      tmp_int_length = st->since_last_sid;                          
      
      if (sub(tmp_int_length, 32) > 0)
      {
         tmp_int_length = 32;                                       
      }
      
      
      st->true_sid_period_inv = div_s(1 << 10, shl(tmp_int_length, 10)); 

      st->since_last_sid = 0;                                       
      Copy(st->lsp, st->lsp_old, M);
      st->old_log_en = st->log_en;                                  
      /* subtract 1/8 in Q11 i.e -6/8 dB */
      st->log_en = sub(st->log_en, 256);                              
   }

   /* reset interpolation length timer 
    * if data has been updated.        */
      
   if ((st->sid_frame != 0) && 
       ((st->valid_data != 0) || 
        ((st->valid_data == 0) &&  (st->dtxHangoverAdded) != 0))) 
   {
      st->since_last_sid =  0;                                      
      st->data_updated = 1;                                         
   }
         
   return 0;
}

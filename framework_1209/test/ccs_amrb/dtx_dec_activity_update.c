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

void dtx_dec_activity_update(dtx_decState *st,
                             Word16 lsf[],
                             Word16 frame[])
{
   Word16 i;

   Word32 L_frame_en;
   Word16 log_en_e, log_en_m, log_en;

   /* update lsp history */
   st->lsf_hist_ptr = add(st->lsf_hist_ptr,M);                     
   
   if (sub(st->lsf_hist_ptr, 80) == 0)
   {
      st->lsf_hist_ptr = 0;                                        
   }
   Copy(lsf, &st->lsf_hist[st->lsf_hist_ptr], M); 

   /* compute log energy based on frame energy */
   L_frame_en = 0;     /* Q0 */                                    
   for (i=0; i < L_FRAME; i++)
   {
      L_frame_en = L_mac(L_frame_en, frame[i], frame[i]); 
   }
   Log2(L_frame_en, &log_en_e, &log_en_m);
   
   /* convert exponent and mantissa to Word16 Q10 */
   log_en = shl(log_en_e, 10);  /* Q10 */                          
   log_en = add(log_en, shr(log_en_m, 15-10));                      
   
   /* divide with L_FRAME i.e subtract with log2(L_FRAME) = 7.32193 */
   log_en = sub(log_en, 7497+1024);                                
   
   /* insert into log energy buffer, no division by two as  *
    * log_en in decoder is Q11                              */
   st->log_en_hist_ptr = add(st->log_en_hist_ptr, 1);
   
   if (sub(st->log_en_hist_ptr, DTX_HIST_SIZE) == 0)
   {
      st->log_en_hist_ptr = 0;                                     
   }
   st->log_en_hist[st->log_en_hist_ptr] = log_en; /* Q11 */        
}


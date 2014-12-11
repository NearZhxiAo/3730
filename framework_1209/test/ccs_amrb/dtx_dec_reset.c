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
/*
*****************************************************************************
*                         LOCAL VARIABLES AND TABLES
*****************************************************************************
*/
#define PN_INITIAL_SEED 0x70816958L   /* Pseudo noise generator seed value  */

/*
**************************************************************************
*
*  Function    : dtx_dec_reset
*
**************************************************************************
*/
int dtx_dec_reset (dtx_decState *st)
{
   int i;

   st->since_last_sid = 0;
   st->true_sid_period_inv = (1 << 13); 
 
   st->log_en = 3500;  
   st->old_log_en = 3500;
   /* low level noise for better performance in  DTX handover cases*/
   
   st->L_pn_seed_rx = PN_INITIAL_SEED;

   /* Initialize state->lsp [] and state->lsp_old [] */
   Copy(lsp_init_data, &st->lsp[0], M);
   Copy(lsp_init_data, &st->lsp_old[0], M);

   st->lsf_hist_ptr = 0;
   st->log_pg_mean = 0;
   st->log_en_hist_ptr = 0;

   /* initialize decoder lsf history */
   Copy(mean_lsf_5, &st->lsf_hist[0], M);

   for (i = 1; i < DTX_HIST_SIZE; i++)
   {
      Copy(&st->lsf_hist[0], &st->lsf_hist[M*i], M);
   }
   Set_zero(st->lsf_hist_mean, M*DTX_HIST_SIZE);

   /* initialize decoder log frame energy */ 
   for (i = 0; i < DTX_HIST_SIZE; i++)
   {
      st->log_en_hist[i] = st->log_en;
   }

   st->log_en_adjust = 0;

   st->dtxHangoverCount = DTX_HANG_CONST;
   st->decAnaElapsedCount = 32767;   

   st->sid_frame = 0;       
   st->valid_data = 0;             
   st->dtxHangoverAdded = 0; 
  
   st->dtxGlobalState = DTX;    
   st->data_updated = 0; 
   return 0;
}

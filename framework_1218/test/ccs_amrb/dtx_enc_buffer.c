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


/*
**************************************************************************
*
*  Function    : dtx_buffer
*  Purpose     : handles the DTX buffer
*                
**************************************************************************
*/
int dtx_enc_buffer(dtx_encState *st,   /* i/o : State struct                    */
	       Word16 lsp_new[],   /* i   : LSP vector                      */
	       Word16 speech[]     /* i   : speech samples                  */
	       )
{
   Word16 i;
   Word32 L_frame_en;
   Word16 log_en_e;
   Word16 log_en_m;
   Word16 log_en;
   
   /* update pointer to circular buffer      */
   st->hist_ptr = add(st->hist_ptr, 1);
   
   if (sub(st->hist_ptr, DTX_HIST_SIZE) == 0)
   {
      st->hist_ptr = 0;                                     
   }
   
   /* copy lsp vector into buffer */
   Copy(lsp_new, &st->lsp_hist[st->hist_ptr * M], M);
   
   /* compute log energy based on frame energy */
   L_frame_en = 0;     /* Q0 */                             
   for (i=0; i < L_FRAME; i++)
   {
      L_frame_en = L_mac(L_frame_en, speech[i], speech[i]); 
   }
   Log2(L_frame_en, &log_en_e, &log_en_m);
   
   /* convert exponent and mantissa to Word16 Q10 */
   log_en = shl(log_en_e, 10);  /* Q10 */
   log_en = add(log_en, shr(log_en_m, 15-10));
   
   /* divide with L_FRAME i.e subtract with log2(L_FRAME) = 7.32193 */
   log_en = sub(log_en, 8521);
   
   /* insert into log energy buffer with division by 2 */
   log_en = shr(log_en, 1);
   st->log_en_hist[st->hist_ptr] = log_en; /* Q10 */        

   return 0;
}

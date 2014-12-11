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
#include "cnst.h"
#include "copy.h"
#include "set_zero.h"
#include "copy.h"
#include "dtx_def.h"
#include "table.h"

#include "cl_ltp_init.h"
#include "cl_ltp_reset.h"
#include "cod_amr.h"
#include "lpc_reset.h"
#include "lsp_reset.h"
#include "gainquant_reset.h"
#include "p_ol_wgh_reset.h"
#include "dtx_enc_reset.h"
#include "ton_stab_reset.h"
#include "vad1_reset.h"
 
/*
**************************************************************************
*
*  Function    : cod_amr_reset
*  Purpose     : Resets state memory
*
**************************************************************************
*/
int cod_amr_reset (cod_amrState *st)
{
   Word16 i;
  
   /*-----------------------------------------------------------------------*
    *          Initialize pointers to speech vector.                        *
    *-----------------------------------------------------------------------*/
      
   st->new_speech = st->old_speech + L_TOTAL - L_FRAME;   /* New speech     */
   
   st->speech = st->new_speech - L_NEXT;                  /* Present frame  */
   
   st->p_window = st->old_speech + L_TOTAL - L_WINDOW;    /* For LPC window */
   st->p_window_12k2 = st->p_window - L_NEXT; /* EFR LPC window: no lookahead */
   
   /* Initialize static pointers */
   
   st->wsp = st->old_wsp + PIT_MAX;
   st->exc = st->old_exc + PIT_MAX + L_INTERPOL;
   st->zero = st->ai_zero + MP1;
   st->error = st->mem_err + M;
   st->h1 = &st->hvec[L_SUBFR];
   
   /* Static vectors to zero */
   
   Set_zero(st->old_speech, L_TOTAL);
   Set_zero(st->old_exc,    PIT_MAX + L_INTERPOL);
   Set_zero(st->old_wsp,    PIT_MAX);
   Set_zero(st->mem_syn,    M);
   Set_zero(st->mem_w,      M);
   Set_zero(st->mem_w0,     M);
   Set_zero(st->mem_err,    M);
   Set_zero(st->zero,       L_SUBFR);
   Set_zero(st->hvec,       L_SUBFR);    /* set to zero "h1[-L_SUBFR..-1]" */

   /* OL LTP states */
   for (i = 0; i < 5; i++)
   {
      st->old_lags[i] = 40; 
   }
   
   /* Reset lpc states */
   lpc_reset(st->lpcSt);
   
   /* Reset lsp states */
   lsp_reset(st->lspSt);
    
   /* Reset clLtp states */
   cl_ltp_reset(st->clLtpSt);
   
   gainQuant_reset(st->gainQuantSt);

   p_ol_wgh_reset(st->pitchOLWghtSt);

   ton_stab_reset(st->tonStabSt);   

   vad1_reset(st->vadSt);
   
   dtx_enc_reset(st->dtx_encSt);

   st->sharp = SHARPMIN;
   
   return 0;
}

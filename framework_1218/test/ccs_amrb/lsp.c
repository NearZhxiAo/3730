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
#include "oper_32b.h"
#include "q_plsf.h"
#include "copy.h"
#include "table.h"
#include "int_lpc_1and3.h"
#include "int_lpc_1to3.h"

#include "az_lsp.h"
#include "lsp.h"
#include "q_plsf_5.h"
#include "q_plsf_3.h"
#include "int_lpc_1and3_2.h"
#include "int_lpc_1to3_2.h"

/*************************************************************************
 *
 *   FUNCTION:  lsp()
 *
 ************************************************************************/
int lsp(lspState *st,        /* i/o : State struct                            */
        enum Mode req_mode,  /* i   : requested coder mode                    */
        enum Mode used_mode, /* i   : used coder mode                         */        
        Word16 az[],         /* i/o : interpolated LP parameters Q12          */
        Word16 azQ[],        /* o   : quantization interpol. LP parameters Q12*/
        Word16 lsp_new[],    /* o   : new lsp vector                          */ 
        Word16 **anap        /* o   : analysis parameters                     */)
{
   Word16 lsp_new_q[M];    /* LSPs at 4th subframe           */
   Word16 lsp_mid[M], lsp_mid_q[M];    /* LSPs at 2nd subframe           */
  
   Word16 pred_init_i; /* init index for MA prediction in DTX mode */

   
   if (req_mode == MR122)
   {
       Az_lsp (&az[MP1], lsp_mid, st->lsp_old);
       Az_lsp (&az[MP1 * 3], lsp_new, lsp_mid);

       /*--------------------------------------------------------------------*
        * Find interpolated LPC parameters in all subframes (both quantized  *
        * and unquantized).                                                  *
        * The interpolated parameters are in array A_t[] of size (M+1)*4     *
        * and the quantized interpolated parameters are in array Aq_t[]      *
        *--------------------------------------------------------------------*/
       Int_lpc_1and3_2 (st->lsp_old, lsp_mid, lsp_new, az);

       
       if ( used_mode != MRDTX)
       {
          /* LSP quantization (lsp_mid[] and lsp_new[] jointly quantized) */
          Q_plsf_5 (st->qSt, lsp_mid, lsp_new, lsp_mid_q, lsp_new_q, *anap);
       
          Int_lpc_1and3 (st->lsp_old_q, lsp_mid_q, lsp_new_q, azQ);
          
          /* Advance analysis parameters pointer */
          (*anap) += 5; 
       }	 
   }
   else
   {
       Az_lsp(&az[MP1 * 3], lsp_new, st->lsp_old);  /* From A(z) to lsp  */
       
       /*--------------------------------------------------------------------*
        * Find interpolated LPC parameters in all subframes (both quantized  *
        * and unquantized).                                                  *
        * The interpolated parameters are in array A_t[] of size (M+1)*4     *
        * and the quantized interpolated parameters are in array Aq_t[]      *
        *--------------------------------------------------------------------*/
       
       Int_lpc_1to3_2(st->lsp_old, lsp_new, az);
       
       
       if (used_mode != MRDTX)
       {
          /* LSP quantization */
          Q_plsf_3(st->qSt, req_mode, lsp_new, lsp_new_q, *anap, &pred_init_i);
          
          Int_lpc_1to3(st->lsp_old_q, lsp_new_q, azQ);
          
          /* Advance analysis parameters pointer */
          (*anap) += 3; 
       }
   }
       
   /* update the LSPs for the next frame */   
   Copy (lsp_new, st->lsp_old, M);
   Copy (lsp_new_q, st->lsp_old_q, M);

   return 0;
}


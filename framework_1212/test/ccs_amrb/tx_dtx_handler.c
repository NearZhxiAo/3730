/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/

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
#include "dtx_def.h"

#include "table.h"

/*
**************************************************************************
*
*  Function    : tx_dtx_handler
*  Purpose     : adds extra speech hangover to analyze speech on the decoding side. 
*                
**************************************************************************
*/
Word16 tx_dtx_handler(dtx_encState *st,      /* i/o : State struct           */
                      Word16 vad_flag,       /* i   : vad decision           */
                      enum Mode *usedMode    /* i/o : mode changed or not    */
                      )
{
   Word16 compute_new_sid_possible;
   
   /* this state machine is in synch with the GSMEFR txDtx machine      */ 
   st->decAnaElapsedCount = add(st->decAnaElapsedCount, 1);  
   
   compute_new_sid_possible = 0;                        

   
   if (vad_flag != 0)
   {
      st->dtxHangoverCount = DTX_HANG_CONST;           
   }
   else 
   {  /* non-speech */
      
      if (st->dtxHangoverCount == 0)
      {  /* out of decoder analysis hangover  */
         st->decAnaElapsedCount = 0;                           
         *usedMode = MRDTX;                             
         compute_new_sid_possible = 1;                  
      }
      else
      { /* in possible analysis hangover */
         st->dtxHangoverCount = sub(st->dtxHangoverCount, 1);
         
         /* decAnaElapsedCount + dtxHangoverCount < DTX_ELAPSED_FRAMES_THRESH */
         
         if (sub(add(st->decAnaElapsedCount, st->dtxHangoverCount),
                 DTX_ELAPSED_FRAMES_THRESH) < 0)
         {
            *usedMode = MRDTX;                          
            /* if short time since decoder update, do not add extra HO */            
         }
         /*
          else 
            override VAD and stay in 
            speech mode *usedMode 
            and add extra hangover
         */
      }
   }
   
   return compute_new_sid_possible;
}

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
#include "lsf_lsp.h"
#include "dtx_dec.h"

/*   
     Table of new SPD synthesis states 
     
                           |     previous SPD_synthesis_state
     Incoming              |  
     frame_type            | SPEECH       | DTX           | DTX_MUTE   
     ---------------------------------------------------------------
     RX_SPEECH_GOOD ,      |              |               |
     RX_SPEECH_PR_DEGRADED | SPEECH       | SPEECH        | SPEECH 
     ----------------------------------------------------------------       
     RX_SPEECH_BAD,        | SPEECH       | DTX           | DTX_MUTE
     ----------------------------------------------------------------
     RX_SID_FIRST,         | DTX          | DTX/(DTX_MUTE)| DTX_MUTE  
     ----------------------------------------------------------------
     RX_SID_UPDATE,        | DTX          | DTX           | DTX
     ----------------------------------------------------------------
     RX_SID_BAD,           | DTX          | DTX/(DTX_MUTE)| DTX_MUTE
     ----------------------------------------------------------------
     RX_NO_DATA,           | SPEECH       | DTX/(DTX_MUTE)| DTX_MUTE
     RX_ONSET              |(class2 garb.)|               |
     ----------------------------------------------------------------
*/

enum DTXStateType rx_dtx_handler(
                      dtx_decState *st,           /* i/o : State struct     */
                      enum RXFrameType frame_type /* i   : Frame type       */ 
                      )
{
   enum DTXStateType newState;
   enum DTXStateType encState;

   /* DTX if SID frame or previously in DTX{_MUTE} and (NO_RX OR BAD_SPEECH) */
     
     
       
   if ((sub(frame_type, RX_SID_FIRST) == 0)   ||
       (sub(frame_type, RX_SID_UPDATE) == 0)  ||
       (sub(frame_type, RX_SID_BAD) == 0)     ||
       (((sub(st->dtxGlobalState, DTX) == 0) ||
         (sub(st->dtxGlobalState, DTX_MUTE) == 0)) && 
        ((sub(frame_type, RX_NO_DATA) == 0) ||
         (sub(frame_type, RX_SPEECH_BAD) == 0) || 
         (sub(frame_type, RX_ONSET) == 0)))) 
   {
      newState = DTX;                                                
      
      /* stay in mute for these input types */
          
      if ((sub(st->dtxGlobalState, DTX_MUTE) == 0) && 
          ((sub(frame_type, RX_SID_BAD) == 0) || 
           (sub(frame_type, RX_SID_FIRST) ==  0) || 
           (sub(frame_type, RX_ONSET) ==  0) || 
           (sub(frame_type, RX_NO_DATA) == 0)))
      {         
         newState = DTX_MUTE;                                             
      }      

      /* evaluate if noise parameters are too old                     */
      /* since_last_sid is reset when CN parameters have been updated */
      st->since_last_sid = add(st->since_last_sid, 1);             
      
      /* no update of sid parameters in DTX for a long while */
      
      if (sub(st->since_last_sid, DTX_MAX_EMPTY_THRESH) > 0)
      {
         newState = DTX_MUTE;                                      
      }
   }
   else 
   {
      newState = SPEECH;                                           
      st->since_last_sid = 0;                                      
   }
   
   /* 
      reset the decAnaElapsed Counter when receiving CNI data the first  
      time, to robustify counter missmatch after handover
      this might delay the bwd CNI analysis in the new decoder slightly.
   */    
    
   if ((st->data_updated == 0) &&
       (sub(frame_type, RX_SID_UPDATE) == 0))
   {
      st->decAnaElapsedCount = 0;                                  
   }

   /* update the SPE-SPD DTX hangover synchronization */
   /* to know when SPE has added dtx hangover         */
   st->decAnaElapsedCount = add(st->decAnaElapsedCount, 1);        
   st->dtxHangoverAdded = 0;                                       
   
        
   if ((sub(frame_type, RX_SID_FIRST) == 0)  ||
       (sub(frame_type, RX_SID_UPDATE) == 0) ||
       (sub(frame_type, RX_SID_BAD) == 0)    ||
       (sub(frame_type, RX_ONSET) == 0)      ||
       (sub(frame_type, RX_NO_DATA) == 0)) 
   {
      encState = DTX;                                                
   }
   else 
   {
      encState = SPEECH;                                           
   }
 
   
   if (sub(encState, SPEECH) == 0)
   {
      st->dtxHangoverCount = DTX_HANG_CONST;                       
   }
   else
   {
      
      if (sub(st->decAnaElapsedCount, DTX_ELAPSED_FRAMES_THRESH) > 0)
      {
         st->dtxHangoverAdded = 1;                                 
         st->decAnaElapsedCount = 0;                               
         st->dtxHangoverCount = 0;                                 
      }
      else if ( st->dtxHangoverCount == 0)
      {
         st->decAnaElapsedCount = 0;                               
      }
      else
      {
         st->dtxHangoverCount = sub(st->dtxHangoverCount, 1);      
      }
   }
   
   if (sub(newState, SPEECH) != 0)
   {
      /* DTX or DTX_MUTE
       * CN data is not in a first SID, first SIDs are marked as SID_BAD 
       *  but will do backwards analysis if a hangover period has been added
       *  according to the state machine above 
       */
      
      st->sid_frame = 0;                                           
      st->valid_data = 0;                                          
            
       
      if (sub(frame_type, RX_SID_FIRST) == 0)
      {
         st->sid_frame = 1;                                        
      }
      else if ( sub(frame_type, RX_SID_UPDATE) == 0)
      {
         st->sid_frame = 1;                                        
         st->valid_data = 1;                                       
      }
      else if ( sub(frame_type, RX_SID_BAD) == 0)
      {
         st->sid_frame = 1;                                        
         st->dtxHangoverAdded = 0; /* use old data */              
      } 
   }

   return newState; 
   /* newState is used by both SPEECH AND DTX synthesis routines */ 
}

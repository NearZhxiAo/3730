/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#ifndef rx_dtx_handler_h
#define rx_dtx_handler_h "$Id $"

#include "typedef.h"
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
                      );

#endif


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
#include "mode.h"
#include "sid_sync.h"

void sid_sync (sid_syncState *st, enum Mode mode,
               enum TXFrameType *tx_frame_type)
{
 
    if ( mode == MRDTX){

       st->sid_update_counter--;
       
        if (st->prev_ft == TX_SPEECH_GOOD) 
        {
           *tx_frame_type = TX_SID_FIRST;
           st->sid_update_counter = 3;
        } 
        else 
        {
           /* TX_SID_UPDATE or TX_NO_DATA */
           if( (st->sid_handover_debt > 0) &&
               (st->sid_update_counter > 2) )
           {
              /* ensure extra updates are  properly delayed after 
                 a possible SID_FIRST */
              *tx_frame_type = TX_SID_UPDATE;
              st->sid_handover_debt--;
           }
           else 
           {
              if (st->sid_update_counter == 0)
              {
                 *tx_frame_type = TX_SID_UPDATE;
                 st->sid_update_counter = st->sid_update_rate;
              } else {
                 *tx_frame_type = TX_NO_DATA;
              }
           }
        }
    }
    else
    {
       st->sid_update_counter = st->sid_update_rate ;
       *tx_frame_type = TX_SPEECH_GOOD;
    }
    st->prev_ft = *tx_frame_type;
}


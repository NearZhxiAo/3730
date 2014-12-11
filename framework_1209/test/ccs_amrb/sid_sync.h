/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#ifndef sid_sync_h
#define sid_sync_h "$Id $"

#include "typedef.h"
#include "mode.h"
#include "frame.h"
 

typedef struct {
    Word16 sid_update_rate;  /* Send SID Update every sid_update_rate frame */
    Word16 sid_update_counter; /* Number of frames since last SID          */
    Word16 sid_handover_debt;  /* Number of extra SID_UPD frames to schedule*/
    enum TXFrameType prev_ft;
} sid_syncState;
 

 
void sid_sync(sid_syncState *st , /* i/o: sid_sync state      */
              enum Mode mode,
              enum TXFrameType *tx_frame_type); 
#endif

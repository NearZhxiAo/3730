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
#include "sid_sync_reset.h"

int sid_sync_init (sid_syncState *state)
{
    state->sid_update_rate = 8;
    return sid_sync_reset(state);
}

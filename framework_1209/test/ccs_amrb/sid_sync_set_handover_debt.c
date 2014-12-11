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
#include "sid_sync.h"

int sid_sync_set_handover_debt (sid_syncState *st,
                                Word16 debtFrames) 
{
   /* debtFrames >= 0 */ 
   st->sid_handover_debt = debtFrames;
   return 0;
}



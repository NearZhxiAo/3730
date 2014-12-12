/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#ifndef dtx_dec_activity_update_h
#define dtx_dec_activity_update_h "$Id $"

#include "typedef.h"
#include "dtx_dec.h"

void dtx_dec_activity_update(dtx_decState *st,
                             Word16 lsf[],
                             Word16 frame[]);
#endif

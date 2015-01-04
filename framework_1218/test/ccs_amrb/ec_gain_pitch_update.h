/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef ec_gain_pitch_update_h
#define ec_gain_pitch_update_h "$Id $"

#include "ec_gain_pitch.h"
#include "typedef.h"

/*
**************************************************************************
*
*  Function    : ec_gain_pitch_update
*  Purpose     : update the pitch gain concealment state;
*                limit gain_pitch if the previous frame was bad
*                Call this function always after decoding (or concealing)
*                the gain
*
**************************************************************************
*/
void ec_gain_pitch_update (
    ec_gain_pitchState *st, /* i/o : state variables                   */
    Word16 bfi,             /* i   : flag: frame is bad                */
    Word16 prev_bf,         /* i   : flag: previous frame was bad      */
    Word16 *gain_pitch      /* i/o : pitch gain                        */
);

#endif

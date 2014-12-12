/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef ec_gain_pitch_h
#define ec_gain_pitch_h "$Id $"

#include "ec_gain_pitch.h"
#include "typedef.h"

typedef struct {
  Word16 pbuf[5];
  Word16 past_gain_pit;
  Word16 prev_gp;
} ec_gain_pitchState;

/*
**************************************************************************
*
*  Function    : ec_gain_pitch
*  Purpose     : conceal the pitch gain
*                Call this function only in BFI (instead of normal gain
*                decoding function)
*
**************************************************************************
*/
void ec_gain_pitch (
    ec_gain_pitchState *st, /* i/o : state variables                   */
    Word16 state,           /* i   : state of the state machine        */
    Word16 *gain_pitch      /* o   : pitch gain (Q14)                  */
);

#endif

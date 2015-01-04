/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#include "ec_gain_pitch.h"
#include "ec_gain_pitch_reset.h"

/*
**************************************************************************
*
*  Function    : ec_gain_pitch_init
*  Purpose     : Allocates memory and initializes state memory.
*
**************************************************************************
*/
int ec_gain_pitch_init (ec_gain_pitchState *state)
{
  ec_gain_pitch_reset(state);
  
  return 0;
}
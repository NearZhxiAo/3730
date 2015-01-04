/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#include "pitch_fr.h"
/*************************************************************************
*
*  Function:   Pitch_fr_reset
*  Purpose:    Initializes state memory to zero
*
**************************************************************************
*/
int Pitch_fr_reset (Pitch_frState *state)
{

    state->T0_prev_subframe = 0;

    return 0;
}

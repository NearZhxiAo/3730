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
#include "pitch_fr_reset.h"
/*************************************************************************
*
*  Function:   Pitch_fr_init
*  Purpose:    Allocates state memory and initializes state memory
*
**************************************************************************
*/
int Pitch_fr_init (Pitch_frState *state)
{
    Pitch_fr_reset(state);

    return 0;
}

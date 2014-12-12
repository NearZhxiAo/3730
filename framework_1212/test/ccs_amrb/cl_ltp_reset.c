/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/

#include "cl_ltp.h"
#include "pitch_fr_reset.h"

/*************************************************************************
*
*  Function:   cl_ltp_reset
*  Purpose:    Initializes state memory to zero
*
**************************************************************************
*/
int cl_ltp_reset (clLtpState *state)
{
    /* Reset pitch search states */
    Pitch_fr_reset (state->pitchSt);
    
    return 0;
}

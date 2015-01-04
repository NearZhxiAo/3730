/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#include "Pitch_ol_wgh.h"
#include "p_ol_wgh_reset.h"
/*************************************************************************
*
*  Function:   p_ol_wgh_init
*  Purpose:    Allocates state memory and initializes state memory
*
**************************************************************************
*/
int p_ol_wgh_init (pitchOLWghtState *state)
{
    p_ol_wgh_reset(state);
    
    return 0;
}

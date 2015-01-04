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
#include "cl_ltp_reset.h"
#include "pitch_fr_init.h"
/*************************************************************************
*
*  Function:   cl_ltp_init
*  Purpose:    Allocates state memory and initializes state memory
*
**************************************************************************
*/
int cl_ltp_init (clLtpState *state)
{
    
    /* allocate memory */
    /* init the sub state */
    Pitch_fr_init(state->pitchSt); 
    
    cl_ltp_reset(state);
    
    return 0;
}
 

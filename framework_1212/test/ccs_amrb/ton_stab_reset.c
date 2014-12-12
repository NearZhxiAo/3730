/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#include "ton_stab.h"
#include "cnst.h"
#include "set_zero.h"

/*************************************************************************
 *
 *  Function:   ton_stab_reset
 *  Purpose:    Initializes state memory to zero
 *
 **************************************************************************
 */
int ton_stab_reset (tonStabState *st)
{
    /* initialize tone stabilizer state */ 
    st->count = 0;
    Set_zero(st->gp, N_FRAME);    /* Init Gp_Clipping */
    
    return 0;
}

/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/

#include "typedef.h"
#include "cnst.h"
#include "gain_adapt.h"

/*************************************************************************
*
*  Function:   gain_adapt_reset
*  Purpose:    Initializes state memory to zero
*
**************************************************************************
*/
int gain_adapt_reset (GainAdaptState *st)
{
    Word16 i;

    st->onset = 0;
    st->prev_alpha = 0;
    st->prev_gc = 0;

    for (i = 0; i < LTPG_MEM_SIZE; i++)
    {
        st->ltpg_mem[i] = 0;
    }

    return 0;
}

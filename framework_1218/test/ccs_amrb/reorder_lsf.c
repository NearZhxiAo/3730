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
#include "basic_op.h"
 
/*************************************************************************
 *
 *  FUNCTION:  Reorder_lsf()
 *
 *  PURPOSE: To make sure that the LSFs are properly ordered and to keep a
 *           certain minimum distance between adjacent LSFs.
 *
 *           The LSFs are in the frequency range 0-0.5 and represented in Q15
 *
 *************************************************************************/
void Reorder_lsf (
    Word16 *lsf,        /* (i/o)     : vector of LSFs   (range: 0<=val<=0.5) */
    Word16 min_dist,    /* (i)       : minimum required distance             */
    Word16 n            /* (i)       : LPC order                             */
)
{
    Word16 i;
    Word16 lsf_min;

    lsf_min = min_dist;          
    for (i = 0; i < n; i++)
    {
         
        if (lsf[i] < lsf_min)
        {
            lsf[i] = lsf_min;    
        }
        lsf_min = add (lsf[i], min_dist);
    }
}

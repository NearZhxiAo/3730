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
#include "oper_32b.h"
#include "cnst.h"
#include "table.h"
#include "inv_sqrt.h"

#include "enc_lag3.h"
#include "enc_lag6.h"
#include "interpol_3or6.h"
#include "convolve.h"
/*************************************************************************
 *
 *  FUNCTION:   searchFrac()
 *
 *  PURPOSE: Find fractional pitch
 *
 *  DESCRIPTION:
 *     The function interpolates the normalized correlation at the
 *     fractional positions around lag T0. The position at which the
 *     interpolation function reaches its maximum is the fractional pitch.
 *     Starting point of the search is frac, end point is last_frac.
 *     frac is overwritten with the fractional pitch.
 *
 *************************************************************************/
void searchFrac (
    Word16 *lag,       /* i/o : integer pitch           */
    Word16 *frac,      /* i/o : start point of search -
                                fractional pitch        */
    Word16 last_frac,  /* i   : endpoint of search      */
    Word16 corr[],     /* i   : normalized correlation  */
    Word16 flag3       /* i   : subsample resolution
                                (3: =1 / 6: =0)         */
)
{
    Word16 i;
    Word16 max;
    Word16 corr_int;

    /* Test the fractions around T0 and choose the one which maximizes   */
    /* the interpolated normalized correlation.                          */

    max = Interpol_3or6 (&corr[*lag], *frac, flag3);  /* function result */

    for (i = add (*frac, 1); i <= last_frac; i++) 
	{
        corr_int = Interpol_3or6 (&corr[*lag], i, flag3);
        
        
        if (corr_int > max) 
		{
            max = corr_int;                       
            *frac = i;                            
        }
    }

    
    if (flag3 == 0) 
	{
        /* Limit the fraction value in the interval [-2,-1,0,1,2,3] */

        
        if (*frac == -3)
		{
            *frac = 3;                            
            *lag = *lag - 1;
        }
    }
    else 
	{
        /* limit the fraction value between -1 and 1 */

        
        if (*frac == -2) 
		{
            *frac = 1;                            
            *lag = *lag - 1;
        }
        
        if (*frac == 2) 
		{
            *frac = -1;                           
            *lag = *lag + 1;
        }
    }
}

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

/*
**************************************************************************
*
*  Function    : autocorr
*  Purpose     : Compute autocorrelations of signal with windowing
*
**************************************************************************
*/
Word16 Autocorr (
    Word16 x[],            /* (i)    : Input signal (L_WINDOW)            */
    Word16 m,              /* (i)    : LPC order                          */
    Word16 r_h[],          /* (o)    : Autocorrelations  (msb)            */
    Word16 r_l[],          /* (o)    : Autocorrelations  (lsb)            */
    const Word16 wind[]    /* (i)    : window for LPC analysis (L_WINDOW) */
)
{
    Word16 i, j, norm;
    Word16 y[L_WINDOW];
    Word32 sum;
    Word16 overfl, overfl_shft;

	long	lTemp;

    /* Windowing of signal */

    for (i = 0; i < L_WINDOW; i++)
    {
        y[i] = mult_r (x[i], wind[i]);  
    }

    /* Compute r[0] and test for cgOverflow */

    overfl_shft = 0;                   

    do
    {
        overfl = 0;                    
        sum = 0L;                      

        for (i = 0; i < L_WINDOW; i++)
        {
            //sum = L_mac (sum, y[i], y[i]);
			lTemp = y[i] * y[i];

			if (lTemp != 0x40000000)
			{
				lTemp = lTemp * 2;
			}
			else
			{
				lTemp = MAX_32;
			}

			lTemp = lTemp + sum;
	
			if (lTemp > MAX_32)
			{
				sum = MAX_32;
			}
			else if (lTemp < MIN_32)
			{
				sum = MIN_32;
			}
			else 
			{
				sum = (Word32)lTemp;
			}
        }

        /* If cgOverflow divide y[] by 4 */

        if (sum == MAX_32)
        {
            overfl_shft = overfl_shft + 4;
            overfl = 1;                /* Set the cgOverflow flag */

            for (i = 0; i < L_WINDOW; i++)
            {
                y[i] = y[i] >> 2;   
            }
        }
        
    }
    while (overfl != 0);

    sum = L_add (sum, 1L);             /* Avoid the case of all zeros */

    /* Normalization of r[0] */

    norm = norm_l (sum);
    sum = L_shl (sum, norm);
    L_Extract (sum, &r_h[0], &r_l[0]); /* Put in DPF format (see oper_32b) */

    /* r[1] to r[m] */

    for (i = 1; i <= m; i++)
    {
        sum = 0;                

        for (j = 0; j < L_WINDOW - i; j++)
        {
            //sum = L_mac (sum, y[j], y[j + i]);
			lTemp = y[j] * y[j + i];

			if (lTemp != 0x40000000)
			{
				lTemp = lTemp * 2;
			}
			else
			{
				lTemp = MAX_32;
			}

			lTemp = lTemp + sum;
	
			if (lTemp > MAX_32)
			{
				sum = MAX_32;
			}
			else if (lTemp < MIN_32)
			{
				sum = MIN_32;
			}
			else 
			{
				sum = (Word32)lTemp;
			}
        }

        sum = L_shl (sum, norm);
        L_Extract (sum, &r_h[i], &r_l[i]);
    }

    norm = norm - overfl_shft;

    return norm;
}
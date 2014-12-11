/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"
#include "inv_sqrt.h"
#include "cnst.h" 
 
/*************************************************************************
 *
 *  FUNCTION:  cor_h()
 *
 *  PURPOSE:  Computes correlations of h[] needed for the codebook search;
 *            and includes the sign information into the correlations.
 *
 *  DESCRIPTION: The correlations are given by
 *         rr[i][j] = sum_{n=i}^{L-1} h[n-i] h[n-j];   i>=j; i,j=0,...,L-1
 *
 *  and the sign information is included by
 *         rr[i][j] = rr[i][j]*sign[i]*sign[j]
 *
 *************************************************************************/

void cor_h (
    Word16 h[],         /* (i) : impulse response of weighted synthesis
                                 filter                                  */
    Word16 sign[],      /* (i) : sign of d[n]                            */
    Word16 rr[][L_CODE] /* (o) : matrix of autocorrelation               */
)
{
    Word16 i, j, k, dec, h2[L_CODE];
    Word32 s;
	long	lTemp;

    /* Scaling for maximum precision */

    s = 2;                                        
    for (i = 0; i < L_CODE; i++)
	{
        //s = L_mac (s, h[i], h[i]);
		lTemp = h[i] * h[i];

		if (lTemp != 0x40000000)
		{
			lTemp = lTemp * 2;
		}
		else
		{
			lTemp = MAX_32;
		}

		lTemp = lTemp + s;

		if (lTemp > MAX_32)
		{
			s = MAX_32;
		}
		else if (lTemp < MIN_32)
		{
			s = MIN_32;
		}
		else 
		{
			s = (Word32)lTemp;
		}
	}
    
    j = sub (extract_h (s), 32767);
     
    if (j == 0)
    {
        for (i = 0; i < L_CODE; i++)
        {
            h2[i] = h[i] >> 1;                
        }
    }
    else
    {
        s = s >> 1;
        k = extract_h (L_shl (Inv_sqrt (s), 7));
        k = mult (k, 32440);                     /* k = 0.99*k */
        
        for (i = 0; i < L_CODE; i++)
        {
            h2[i] = round (L_shl (L_mult (h[i], k), 9));
                                                  
        }
    }
    
    /* build matrix rr[] */
    s = 0;                                        
    i = L_CODE - 1;
    for (k = 0; k < L_CODE; k++, i--)
    {
        //s = L_mac (s, h2[k], h2[k]);
		lTemp = h2[k] * h2[k];
		if (lTemp != 0x40000000)
		{
			lTemp = lTemp * 2;
		}
		else
		{
			lTemp = MAX_32;
		}

		lTemp = lTemp + s;

		if (lTemp > MAX_32)
		{
			s = MAX_32;
		}
		else if (lTemp < MIN_32)
		{
			s = MIN_32;
		}
		else 
		{
			s = (Word32)lTemp;
		}

        rr[i][i] = round (s);                     
    }
    
    for (dec = 1; dec < L_CODE; dec++)
    {
        s = 0;                                    
        j = L_CODE - 1;
        i = sub (j, dec);
        for (k = 0; k < (L_CODE - dec); k++, i--, j--)
        {
            //s = L_mac (s, h2[k], h2[k + dec]);
			lTemp = h2[k] * h2[k + dec];
			if (lTemp != 0x40000000)
			{
				lTemp = lTemp * 2;
			}
			else
			{
				lTemp = MAX_32;
			}

			lTemp = lTemp + s;

			if (lTemp > MAX_32)
			{
				s = MAX_32;
			}
			else if (lTemp < MIN_32)
			{
				s = MIN_32;
			}
			else 
			{
				s = (Word32)lTemp;
			}
			
            rr[j][i] = mult (round (s), mult (sign[i], sign[j]));
                                                  
            rr[i][j] = rr[j][i];                  
        }
    }
}

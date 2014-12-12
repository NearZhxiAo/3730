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
#include "cnst.h" 
/*************************************************************************
 *
 *  FUNCTION:   Convolve
 *
 *  PURPOSE:
 *     Perform the convolution between two vectors x[] and h[] and
 *     write the result in the vector y[]. All vectors are of length L
 *     and only the first L samples of the convolution are computed.
 *
 *  DESCRIPTION:
 *     The convolution is given by
 *
 *          y[n] = sum_{i=0}^{n} x[i] h[n-i],        n=0,...,L-1
 *
 *************************************************************************/
void Convolve (
    Word16 x[],        /* (i)     : input vector                           */
    Word16 h[],        /* (i)     : impulse response                       */
    Word16 y[],        /* (o)     : output vector                          */
    Word16 L           /* (i)     : vector size                            */
)
{
    Word16 i, n;
    Word32 s;

	long	lTemp;

    for (n = 0; n < L; n++)
    {
        s = 0;                   
        for (i = 0; i <= n; i++)
        {
            //s = L_mac (s, x[i], h[n - i]);
			lTemp = x[i] * h[n - i];

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
        s = L_shl (s, 3);
        y[n] = extract_h (s);    
    }

    return;
}

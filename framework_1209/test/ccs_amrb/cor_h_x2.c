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
 *  FUNCTION:  cor_h_x2()
 *
 *  PURPOSE:  Computes correlation between target signal "x[]" and
 *            impulse response"h[]".
 *
 *  DESCRIPTION:
 *            See cor_h_x, d[n] can be normalized regards to sum of the
 *            five MR122 maxima or the four MR102 maxima.
 *
 *************************************************************************/
void cor_h_x2 (
    Word16 h[],    /* (i): impulse response of weighted synthesis filter */
    Word16 x[],    /* (i): target                                        */
    Word16 dn[],   /* (o): correlation between target and h[]            */
    Word16 sf,     /* (i): scaling factor: 2 for 12.2, 1 for others      */
    Word16 nb_track,/* (i): the number of ACB tracks                     */
    Word16 step    /* (i): step size from one pulse position to the next
                           in one track                                  */
)
{
    Word16 i, j, k;
    Word32 s, y32[L_CODE], max, tot;

	long	lTemp;

    /* first keep the result on 32 bits and find absolute maximum */

    tot = 5;                                      

    for (k = 0; k < nb_track; k++)
    {
        max = 0;                                  
        for (i = k; i < L_CODE; i += step)
        {
            s = 0;                                
            for (j = i; j < L_CODE; j++)
			{
                //s = L_mac (s, x[j], h[j - i]);
				lTemp = x[j] * h[j-i];

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
            
            y32[i] = s;                           
            
            s = L_abs (s);
             
            if (s > max)
                max = s;                          
        }
        tot = L_add (tot, (max >> 1));
    }
    
    j = sub (norm_l (tot), sf);
    
    for (i = 0; i < L_CODE; i++)
    {
        dn[i] = round (L_shl (y32[i], j));        
    }
}

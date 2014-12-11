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

#include "interpol_3or6.h"
#include "convolve.h"
/*************************************************************************
 *
 *  FUNCTION:   Norm_Corr()
 *
 *  PURPOSE: Find the normalized correlation between the target vector
 *           and the filtered past excitation.
 *
 *  DESCRIPTION:
 *     The normalized correlation is given by the correlation between the
 *     target and filtered past excitation divided by the square root of
 *     the energy of filtered excitation.
 *                   corr[k] = <x[], y_k[]>/sqrt(y_k[],y_k[])
 *     where x[] is the target vector and y_k[] is the filtered past
 *     excitation at delay k.
 *
 *************************************************************************/
void Norm_Corr (Word16 exc[], Word16 xn[], Word16 h[], Word16 L_subfr,
                       Word16 t_min, Word16 t_max, Word16 corr_norm[])
{
    Word16 i, j, k;
    Word16 corr_h, corr_l, norm_h, norm_l;
    Word32 s;

    /* Usally dynamic allocation of (L_subfr) */
    Word16 excf[L_SUBFR];
    Word16 scaling, h_fac, *s_excf, scaled_excf[L_SUBFR];

	long	lTemp;

    k = -t_min;                                

    /* compute the filtered excitation for the first delay t_min */

    Convolve (&exc[k], h, excf, L_subfr);

    /* scale "excf[]" to avoid cgOverflow */

    for (j = 0; j < L_subfr; j++) 
	{
        scaled_excf[j] = excf[j] >> 2;     
    }

    /* Compute 1/sqrt(energy of excf[]) */

    s = 0;                                     
    for (j = 0; j < L_subfr; j++) 
	{
        //s = L_mac (s, excf[j], excf[j]);
		lTemp = excf[j] * excf[j];

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
    
    if (s <= 67108864L) 
	{            /* if (s <= 2^26) */
        s_excf = excf;                         
        h_fac = 15 - 12;                       
        scaling = 0;                           
    }
    else 
	{
        /* "excf[]" is divided by 2 */
        s_excf = scaled_excf;                  
        h_fac = 15 - 12 - 2;                   
        scaling = 2;                           
    }

    /* loop for every possible period */

    for (i = t_min; i <= t_max; i++) 
	{
        /* Compute 1/sqrt(energy of excf[]) */
        
        s = 0;                                 
        for (j = 0; j < L_subfr; j++) 
		{
            //s = L_mac (s, s_excf[j], s_excf[j]);
			lTemp = s_excf[j] * s_excf[j];

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
        
        s = Inv_sqrt (s);
        L_Extract (s, &norm_h, &norm_l);
        
        /* Compute correlation between xn[] and excf[] */
        
        s = 0;                                  
        for (j = 0; j < L_subfr; j++) 
		{
            //s = L_mac (s, xn[j], s_excf[j]);
			lTemp = xn[j] * s_excf[j];

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
        L_Extract (s, &corr_h, &corr_l);

        /* Normalize correlation = correlation * (1/sqrt(energy)) */
        
        s = Mpy_32 (corr_h, corr_l, norm_h, norm_l);
        
        corr_norm[i] = extract_h (L_shl (s, 16));
        

            /* modify the filtered excitation excf[] for the next iteration */
        
        
        if (i != t_max)
		{
            k--;
            for (j = L_subfr - 1; j > 0; j--) 
			{
                s = L_mult (exc[k], h[j]);
                s = L_shl (s, h_fac);
                s_excf[j] = add (extract_h (s), s_excf[j - 1]); 
            }
            s_excf[0] = shr (exc[k], scaling);  
        }
    }
    return;
}

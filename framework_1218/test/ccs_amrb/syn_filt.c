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

void Syn_filt (
    Word16 a[],     /* (i)     : a[M+1] prediction coefficients   (M=10)  */
    Word16 x[],     /* (i)     : input signal                             */
    Word16 y[],     /* (o)     : output signal                            */
    Word16 lg,      /* (i)     : size of filtering                        */
    Word16 mem[],   /* (i/o)   : memory associated with this filtering.   */
    Word16 update   /* (i)     : 0=no update, 1=update of memory.         */
)
{
    Word16 i, j;
    Word32 s;
    Word16 tmp[80];   /* This is usually done by memory allocation (lg+M) */
    Word16 *yy;

	long	Temp;

    /* Copy mem[] to yy[] */

    yy = tmp;                            

    for (i = 0; i < M; i++)
    {
        *yy++ = mem[i];                  
    } 

    /* Do the filtering. */

    for (i = 0; i < lg; i++)
    {
        Temp = x[i] * a[0];
		if (Temp != (Word32) 0x40000000)
		{
			Temp = Temp * 2;
		}
		else
		{
			cgOverflow = 1;
			Temp = MAX_32;
		}
		s = Temp;
        for (j = 1; j <= M; j++)
        {
	        Temp = a[j] * yy[-j];
			if (Temp != (Word32) 0x40000000)
			{
				Temp = Temp * 2;
			}
			else
			{
				cgOverflow = 1;
				Temp = MAX_32;
			}

			Temp = s - Temp;
			if (Temp >= MAX_32)
			{
				cgOverflow = 1;
				s = MAX_32;
			}
			else if (Temp <= MIN_32)
			{
				cgOverflow = 1;
				s = MIN_32;
			}
			else
			{
				s = Temp;
			}
        }
        Temp = s << 3;
		if (Temp >= MAX_32)
		{
			cgOverflow = 1;
			s = MAX_32;
		}
		else if (Temp <= MIN_32)
		{
			cgOverflow = 1;
			s = MIN_32;
		}
		else
		{
			s = Temp;
		}

		Temp = s + 0x08000;
		if (Temp >= MAX_32)
		{
			cgOverflow = 1;
			s = MAX_32;
		}
		else if (Temp <= MIN_32)
		{
			cgOverflow = 1;
			s = MIN_32;
		}
		else
		{
			s = Temp;
		}
		s = s >> 16;

        *yy++ = (Word16)s;
    }

    for (i = 0; i < lg; i++)
    {
        y[i] = tmp[i + M];               
    }

    /* Update of memory if update==1 */

     
    if (update != 0)
    {
        for (i = 0; i < M; i++)
        {
            mem[i] = y[lg - M + i];      
        }
    }
    return;
}

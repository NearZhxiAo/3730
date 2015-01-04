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

void Residu (
    Word16 a[], /* (i)     : prediction coefficients                      */
    Word16 x[], /* (i)     : speech signal                                */
    Word16 y[], /* (o)     : residual signal                              */
    Word16 lg   /* (i)     : size of filtering                            */
)
{
    Word16 i, j;
    Word32 s;
	long int Temp;

    for (i = 0; i < lg; i++)
    {
        Temp = x[i] * a[0];
		if (Temp != (Word32) 0x40000000)
		{
			s = Temp * 2;
		}
		else
		{
			cgOverflow = 1;
			s = MAX_32;
		}

        for (j = 1; j <= M; j++)
        {
	        Temp = a[j] * x[i - j];
			if (Temp != (Word32) 0x40000000)
			{
				Temp = Temp * 2;
			}
			else
			{
				cgOverflow = 1;
				Temp = MAX_32;
			}

			Temp = s + Temp;
			if (Temp >= MAX_32)
			{
				s = MAX_32;
			}
			else if (Temp <= MIN_32)
			{
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
			s = MAX_32;
		}
		else if (Temp <= MIN_32)
		{
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

        y[i] = (Word16)s;        
    }
    return;
}

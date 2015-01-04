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
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/
/*************************************************************************
 *
 *  FUNCTION: comp_corr 
 *
 *  PURPOSE: Calculate all correlations of scal_sig[] in a given delay
 *           range.
 *
 *  DESCRIPTION:
 *      The correlation is given by
 *           cor[t] = <scal_sig[n],scal_sig[n-t]>,  t=lag_min,...,lag_max
 *      The functions outputs the all correlations 
 *
 *************************************************************************/
void comp_corr ( 
    Word16 scal_sig[],  /* i   : scaled signal.                          */
    Word16 L_frame,     /* i   : length of frame to compute pitch        */
    Word16 lag_max,     /* i   : maximum lag                             */
    Word16 lag_min,     /* i   : minimum lag                             */
    Word32 corr[])      /* o   : correlation of selected lag             */
{
    Word16 i, j;
    Word16 *p, *p1;
    Word32 t0;

	long lTemp;
    
    for (i = lag_max; i >= lag_min; i--)
    {
       p = scal_sig;            
       p1 = &scal_sig[-i];      
       t0 = 0;                  
       
       for (j = 0; j < L_frame; j++, p++, p1++)
       {
		   lTemp = (*p) * (*p1);
		   if (lTemp != (Word32) 0x40000000)
		   {
			   lTemp = lTemp * 2;
		   }
		   else
		   {
			   cgOverflow = 1;
			   lTemp = MAX_32;
		   }
		   lTemp = t0 + lTemp;

		   if (lTemp >= MAX_32)
		   {
			   cgOverflow = 1;
			   t0 = MAX_32;
		   }
		   else if (lTemp <= MIN_32)
		   {
			   cgOverflow = 1;
			   t0 = MIN_32;
		   }
		   else
		   {
			   t0 = lTemp;
		   }
       }
       corr[-i] = t0;          
    }

    return;
}

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
#include "inv_sqrt.h"
#include "vad1.h"
#include "comp_corr.h"
#include "hp_max.h"
#include "vad_tone_detection.h" 
/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/
#define THRESHOLD 27853

/*************************************************************************
 *
 *  FUNCTION:  Lag_max
 *
 *  PURPOSE: Find the lag that has maximum correlation of scal_sig[] in a
 *           given delay range.
 *
 *  DESCRIPTION:
 *      The correlation is given by
 *           cor[t] = <scal_sig[n],scal_sig[n-t]>,  t=lag_min,...,lag_max
 *      The functions outputs the maximum correlation after normalization
 *      and the corresponding lag.
 *
 *************************************************************************/
Word16 Lag_max ( /* o   : lag found                               */
    vadState1 *vadSt,    /* i/o : VAD state struct                        */
    Word32 corr[],      /* i   : correlation vector.                     */
    Word16 scal_sig[],  /* i   : scaled signal.                          */    
    Word16 scal_fac,    /* i   : scaled signal factor.                   */
    Word16 scal_flag,   /* i   : if 1 use EFR compatible scaling         */
    Word16 L_frame,     /* i   : length of frame to compute pitch        */
    Word16 lag_max,     /* i   : maximum lag                             */
    Word16 lag_min,     /* i   : minimum lag                             */
    Word16 *cor_max,    /* o   : normalized correlation of selected lag  */
    Flag dtx            /* i   : dtx flag; use dtx=1, do not use dtx=0   */
    )
{
    Word16 i, j;
    Word16 *p;
    Word32 max, t0;
    Word16 max_h, max_l, ener_h, ener_l;
    Word16 p_max = 0; /* initialization only needed to keep gcc silent */
    
    long lTemp;

	max = MIN_32;                
    p_max = lag_max;            
   
    for (i = lag_max, j = (PIT_MAX-lag_max-1); i >= lag_min; i--, j--)  
    {
         
       if (corr[-i] >= max) 
       { 
          max = corr[-i];         
          p_max = i;              
       } 
    }
    
    /* compute energy */

    t0 = 0;                          
    p = &scal_sig[-p_max];       
    for (i = 0; i < L_frame; i++, p++)
    {
        //t0 = L_mac (t0, *p, *p);
		lTemp = (*p) * (*p);

		if (lTemp != 0x40000000)
		{
			lTemp = lTemp * 2;
		}
		else
		{
			lTemp = MAX_32;
		}

		lTemp = lTemp + t0;

		if (lTemp > MAX_32)
		{
			t0 = MAX_32;
		}
		else if (lTemp < MIN_32)
		{
			t0 = MIN_32;
		}
		else 
		{
			t0 = (Word32)lTemp;
		}

    }
    /* 1/sqrt(energy) */

    if (dtx)
    {  /* no test() call since this if is only in simulation env */
       /* check tone */
       vad_tone_detection (vadSt, max, t0);
    }
    
    t0 = Inv_sqrt (t0);  /* function result */

    
    if (scal_flag)
    {
       t0 = L_shl (t0, 1);
    }
    
    /* max = max/sqrt(energy)  */

    L_Extract (max, &max_h, &max_l);
    L_Extract (t0, &ener_h, &ener_l);

    t0 = Mpy_32 (max_h, max_l, ener_h, ener_l);
    
    
    if (scal_flag)
    {
      t0 = L_shr (t0, scal_fac);
      *cor_max = extract_h (L_shl (t0, 15)); /* divide by 2 */
    }
    else
    {
      *cor_max = extract_l(t0);
    }

    return (p_max);
}

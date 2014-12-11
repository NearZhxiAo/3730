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
#include "oper_32b.h"
#include "cnst.h"
#include "table.h"
#include "gmed_n.h"
#include "inv_sqrt.h"
#include "vad1.h"
#include "comp_corr.h"
#include "hp_max.h"
#include "vad_tone_detection.h"
#include "vad_tone_detection_update.h"
/*************************************************************************
 *
 *  FUNCTION:  Lag_max_wgh
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
Word16 Lag_max_wgh ( /* o : lag found                               */
    vadState1 *vadSt,    /* i/o : VAD state struct                      */
    Word32 corr[],      /* i   : correlation vector.                   */
    Word16 scal_sig[],  /* i : scaled signal.                          */
    Word16 L_frame,     /* i : length of frame to compute pitch        */
    Word16 lag_max,     /* i : maximum lag                             */
    Word16 lag_min,     /* i : minimum lag                             */
    Word16 old_lag,     /* i : old open-loop lag                       */
    Word16 *cor_max,    /* o : normalized correlation of selected lag  */
    Word16 wght_flg,    /* i : is weighting function used              */ 
    Word16 *gain_flg,   /* o : open-loop flag                          */
    Flag dtx            /* i   : dtx flag; use dtx=1, do not use dtx=0 */
    )
{
    Word16 i, j;
    Word16 *p, *p1;
    Word32 max, t0;
    Word16 t0_h, t0_l;
    Word16 p_max;
    const Word16 *ww, *we;
    Word32 t1;

	long lTemp;
    
    ww = &corrweight[250];                                 
    we = &corrweight[123 + lag_max - old_lag];             

    max = MIN_32;                                          
    p_max = lag_max;                                       

    for (i = lag_max; i >= lag_min; i--)
    {
       t0 = corr[-i];                                         
       
       /* Weighting of the correlation function.   */
       L_Extract (corr[-i], &t0_h, &t0_l);
       t0 = Mpy_32_16 (t0_h, t0_l, *ww);
       ww--;                                               
       
       if (wght_flg > 0) 
	   {
          /* Weight the neighbourhood of the old lag. */
          L_Extract (t0, &t0_h, &t0_l);
          t0 = Mpy_32_16 (t0_h, t0_l, *we);
          we--;                                            
       }
       
        
       if (t0 >= max)
       {
          max = t0;                                         
          p_max = i;                                        
       }
    }
    
    p  = &scal_sig[0];                                      
    p1 = &scal_sig[-p_max];                                 
    t0 = 0;                                                 
    t1 = 0;                                                 
    
    for (j = 0; j < L_frame; j++, p++, p1++)
    {
       //t0 = L_mac (t0, *p, *p1);     
		lTemp = (*p) * (*p1);

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
   
       //t1 = L_mac (t1, *p1, *p1);
   		lTemp = (*p1) * (*p1);

		if (lTemp != 0x40000000)
		{
			lTemp = lTemp * 2;
		}
		else
		{
			lTemp = MAX_32;
		}

		lTemp = lTemp + t1;

		if (lTemp > MAX_32)
		{
			t1 = MAX_32;
		}
		else if (lTemp < MIN_32)
		{
			t1 = MIN_32;
		}
		else 
		{
			t1 = (Word32)lTemp;
		}

    }

    if (dtx)
    {  /* no test() call since this if is only in simulation env */
       /* update and detect tone */
       vad_tone_detection_update (vadSt, 0);
       vad_tone_detection (vadSt, t0, t1);

    }
    
    /* gain flag is set according to the open_loop gain */
    /* is t2/t1 > 0.4 ? */    
    *gain_flg = round(L_msu(t0, round(t1), 13107));         
    
    *cor_max = 0;                                          

    return (p_max);
}


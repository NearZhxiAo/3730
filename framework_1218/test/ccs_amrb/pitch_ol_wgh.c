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
#include "pitch_ol_wgh.h"
#include "lag_max_wgh.h"
#include "vad_complex_detection_update.h"
                      
/*************************************************************************
*
*  Function:   Pitch_ol_wgh
*  Purpose:    open-loop pitch search with weighting
*
**************************************************************************
*/
Word16 Pitch_ol_wgh (     /* o   : open loop pitch lag                            */
    pitchOLWghtState *st, /* i/o : State struct                                   */
    vadState1 *vadSt,      /* i/o : VAD state struct                               */
    Word16 signal[],      /* i   : signal used to compute the open loop pitch     */
                          /*       signal[-pit_max] to signal[-1] should be known */
    Word16 pit_min,       /* i   : minimum pitch lag                              */
    Word16 pit_max,       /* i   : maximum pitch lag                              */
    Word16 L_frame,       /* i   : length of frame to compute pitch               */
    Word16 old_lags[],    /* i   : history with old stored Cl lags                */
    Word16 ol_gain_flg[], /* i   : OL gain flag                                   */
    Word16 idx,           /* i   : index                                          */
    Flag dtx              /* i   : dtx flag; use dtx=1, do not use dtx=0          */
    )
{
    Word16 i;
    Word16 max1;
    Word16 p_max1;
    Word32 t0;

    Word16 corr_hp_max;

    Word32 corr[PIT_MAX+1], *corr_ptr;    

    /* Scaled signal */
    Word16 scaled_signal[PIT_MAX + L_FRAME];
    Word16 *scal_sig;
	
	long   lTemp;

    scal_sig = &scaled_signal[pit_max];                           

    t0 = 0L;                                                      
    for (i = -pit_max; i < L_frame; i++)
    {
        //t0 = L_mac (t0, signal[i], signal[i]);
		lTemp = signal[i] * signal[i];

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
    /*--------------------------------------------------------*
     * Scaling of input signal.                               *
     *                                                        *
     *   if cgOverflow        -> scal_sig[i] = signal[i]>>2     *
     *   else if t0 < 1^22  -> scal_sig[i] = signal[i]<<2     *
     *   else               -> scal_sig[i] = signal[i]        *
     *--------------------------------------------------------*/

    /*--------------------------------------------------------*
     *  Verification for risk of cgOverflow.                    *
     *--------------------------------------------------------*/

      
    if (t0 == MAX_32)               /* Test for cgOverflow */
    {
        for (i = -pit_max; i < L_frame; i++)
        {
            scal_sig[i] = signal[i] >> 3;    
        }
    }
    else if (t0 < 1048576L)
    {
        for (i = -pit_max; i < L_frame; i++)
        {
            scal_sig[i] = shl (signal[i], 3);    
        }
    }
    else
    {
        for (i = -pit_max; i < L_frame; i++)
        {
            scal_sig[i] = signal[i];             
        }
    }

    /* calculate all coreelations of scal_sig, from pit_min to pit_max */
    corr_ptr = &corr[pit_max];                  
    comp_corr (scal_sig, L_frame, pit_max, pit_min, corr_ptr); 

    p_max1 = Lag_max_wgh (vadSt, corr_ptr, scal_sig, L_frame, pit_max, pit_min,
                      st->old_T0_med, &max1, st->wght_flg, &ol_gain_flg[idx],
                      dtx);
    

     
    if (ol_gain_flg[idx] > 0)
    {
       /* Calculate 5-point median of previous lags */
       for (i = 4; i > 0; i--) /* Shift buffer */
       {
          old_lags[i] = old_lags[i-1];              
       }
       old_lags[0] = p_max1;                         
       st->old_T0_med = gmed_n (old_lags, 5);       
       st->ada_w = 32767;                            /* Q15 = 1.0 */
    }	
    else
    {        
       st->old_T0_med = p_max1;                     
       st->ada_w = mult(st->ada_w, 29491);      /* = ada_w = ada_w * 0.9 */
    }
    
    
    if (st->ada_w < 9830)  /* ada_w - 0.3 */
    { 
       st->wght_flg = 0;                            
    } 
    else
    {
       st->wght_flg = 1;                            
    }

    if (dtx)
    {  /* no test() call since this if is only in simulation env */
       
       if (idx == 1)
       {
          /* calculate max high-passed filtered correlation of all lags */
          hp_max (corr_ptr, scal_sig, L_frame, pit_max, pit_min, &corr_hp_max); 
          
          /* update complex background detector */
          vad_complex_detection_update(vadSt, corr_hp_max); 
       }
    }

    
    return (p_max1);
}


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
#include "mode.h"
#include "vad1.h"
#include "comp_corr.h"
#include "hp_max.h"
#include "lag_max.h"
#include "vad_tone_detection_update.h"
#include "vad_complex_detection_update.h" 
/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/
#define THRESHOLD 27853

/*************************************************************************
 *
 *  FUNCTION:  Pitch_ol
 *
 *  PURPOSE: Compute the open loop pitch lag.
 *
 *  DESCRIPTION:
 *      The open-loop pitch lag is determined based on the perceptually
 *      weighted speech signal. This is done in the following steps:
 *        - find three maxima of the correlation <sw[n],sw[n-T]>,
 *          dividing the search range into three parts:
 *               pit_min ... 2*pit_min-1
 *             2*pit_min ... 4*pit_min-1
 *             4*pit_min ...   pit_max
 *        - divide each maximum by <sw[n-t], sw[n-t]> where t is the delay at
 *          that maximum correlation.
 *        - select the delay of maximum normalized correlation (among the
 *          three candidates) while favoring the lower delay ranges.
 *
 *************************************************************************/
Word16 Pitch_ol (      /* o   : open loop pitch lag                         */
    vadState1 *vadSt,   /* i/o : VAD state struct                            */
    enum Mode mode,    /* i   : coder mode                                  */
    Word16 signal[],   /* i   : signal used to compute the open loop pitch  */
                       /*    signal[-pit_max] to signal[-1] should be known */
    Word16 pit_min,    /* i   : minimum pitch lag                           */
    Word16 pit_max,    /* i   : maximum pitch lag                           */
    Word16 L_frame,    /* i   : length of frame to compute pitch            */
    Word16 idx,        /* i   : frame index                                 */
    Flag dtx           /* i   : dtx flag; use dtx=1, do not use dtx=0       */
    )
{
    Word16 i, j;
    Word16 max1, max2, max3;
    Word16 p_max1, p_max2, p_max3;
    Word16 scal_flag = 0;
    Word32 t0;

    Word16 corr_hp_max;

    Word32 corr[PIT_MAX+1], *corr_ptr;
    
    /* Scaled signal */

    Word16 scaled_signal[L_FRAME + PIT_MAX];
    Word16 *scal_sig, scal_fac;

	long lTemp;

    if (dtx)
    {  /* no test() call since this if is only in simulation env */
       /* update tone detection */
        
       if ((mode == MR475) || (mode == MR515))
       {
          vad_tone_detection_update (vadSt, 1);
       }
       else
       {
          vad_tone_detection_update (vadSt, 0);
       }
    }
    
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
     *   if cgOverflow        -> scal_sig[i] = signal[i]>>3     *
     *   else if t0 < 1^20  -> scal_sig[i] = signal[i]<<3     *
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
        scal_fac = 3;                            
    }
    else if (t0 < 1048576L)
        /* if (t0 < 2^20) */
    {
		 
        for (i = -pit_max; i < L_frame; i++)
        {
            scal_sig[i] = shl (signal[i], 3);    
        }
        scal_fac = -3;                           
    }
    else
    {
		 
        for (i = -pit_max; i < L_frame; i++)
        {
            scal_sig[i] = signal[i];             
        }
        scal_fac = 0;                            
    }

    /* calculate all coreelations of scal_sig, from pit_min to pit_max */
    corr_ptr = &corr[pit_max];                  
    comp_corr (scal_sig, L_frame, pit_max, pit_min, corr_ptr); 
    
    /*--------------------------------------------------------------------*
     *  The pitch lag search is divided in three sections.                *
     *  Each section cannot have a pitch multiple.                        *
     *  We find a maximum for each section.                               *
     *  We compare the maximum of each section by favoring small lags.    *
     *                                                                    *
     *  First section:  lag delay = pit_max     downto 4*pit_min          *
     *  Second section: lag delay = 4*pit_min-1 downto 2*pit_min          *
     *  Third section:  lag delay = 2*pit_min-1 downto pit_min            *
     *--------------------------------------------------------------------*/

    /* mode dependent scaling in Lag_max */
     
    if (mode == MR122)
    {
       scal_flag = 1;                            
    }
    else
    {
       scal_flag = 0;                               
    } 
    
    j = shl (pit_min, 2);
    p_max1 = Lag_max (vadSt, corr_ptr, scal_sig, scal_fac, scal_flag, L_frame,
                      pit_max, j, &max1, dtx);   /* function result */

    i = sub (j, 1);
    j = shl (pit_min, 1);
    p_max2 = Lag_max (vadSt, corr_ptr, scal_sig, scal_fac, scal_flag, L_frame,
                      i, j, &max2, dtx);         /* function result */

    i = sub (j, 1);
    p_max3 = Lag_max (vadSt, corr_ptr, scal_sig, scal_fac, scal_flag, L_frame,
                      i, pit_min, &max3, dtx);   /* function result */

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
    
    /*--------------------------------------------------------------------*
     * Compare the 3 sections maximum, and favor small lag.               *
     *--------------------------------------------------------------------*/
    
     
    if (sub (mult (max1, THRESHOLD), max2) < 0)
    {
        max1 = max2;                        
        p_max1 = p_max2;                    
    }
     
    if (sub (mult (max1, THRESHOLD), max3) < 0)
    {
        p_max1 = p_max3;                    
    }


    return (p_max1);
}



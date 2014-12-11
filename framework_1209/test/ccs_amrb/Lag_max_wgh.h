/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#ifndef Lag_max_wgh_h
#define Lag_max_wgh_h "$Id $"

#include "typedef.h"
#include "vad1.h"
                      
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
    );
#endif

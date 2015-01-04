/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef comp_corr_h
#define comp_corr_h "$Id $"
 
#include "typedef.h"
/*************************************************************************
 *
 *  FUNCTION: comp_corr 
 *
 *  PURPOSE: Calculate all correlations of scal_sig[] in a given delay
 *           range.
 *
 *  DESCRIPTION:
 *      The correlation is given by
 *           cor[t] = <scal_sig[n], scal_sig[n-t]>,  t=lag_min,...,lag_max
 *      The functions outputs all correlations in the given range 
 *
 *************************************************************************/
void comp_corr (Word16 scal_sig[],  /* i   : scaled signal.                     */
                Word16 L_frame,     /* i   : length of frame to compute pitch   */
                Word16 lag_max,     /* i   : maximum lag                        */
                Word16 lag_min,     /* i   : minimum lag                        */
                Word32 corr[]       /* o   : correlation of selected lag        */
                );
#endif

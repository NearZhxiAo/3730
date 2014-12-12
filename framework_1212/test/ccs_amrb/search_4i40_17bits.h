/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef search_4i40_17bits_h
#define search_4i40_17bits_h "$Id $"

#include "typedef.h"
#include "cnst.h"

/*************************************************************************
 *
 *  FUNCTION  search_4i40_17bits()
 *
 *  PURPOSE: Search the best codevector; determine positions of the 4 pulses
 *           in the 40-sample frame.
 *
 *************************************************************************/
void search_4i40_17bits(
    Word16 dn[],         /* i : correlation between target and h[]  */
    Word16 dn2[],        /* i : maximum of corr. in each track.     */
    Word16 rr[][L_CODE], /* i : matrix of autocorrelation           */
    Word16 codvec[]      /* o : algebraic codebook vector           */
);
#endif

/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef search_3i40_14bits_h
#define search_3i40_14bits_h "$Id $"

#include "typedef.h"
#include "cnst.h"

/*************************************************************************
 *
 *  FUNCTION  search_3i40_14bits()
 *
 *  PURPOSE: Search the best codevector; determine positions of the 3 pulses
 *           in the 40-sample frame.
 *
 *************************************************************************/
void search_3i40_14bits(
    Word16 dn[],         /* i : correlation between target and h[] */
    Word16 dn2[],        /* i : maximum of corr. in each track.    */
    Word16 rr[][L_CODE], /* i : matrix of autocorrelation          */
    Word16 codvec[]      /* o : algebraic codebook vector          */
);
#endif

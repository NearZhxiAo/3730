/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/

#ifndef search_2i40_9bits_h
#define search_2i40_9bits_h "$Id $"

#include "typedef.h"
#include "cnst.h"

/*************************************************************************
 *
 *  FUNCTION  search_2i40_9bits()
 *
 *  PURPOSE: Search the best codevector; determine positions of the 2 pulses
 *           in the 40-sample frame.
 *
 *************************************************************************/
void search_2i40_9bits(
    Word16 subNr,        /* i : subframe number                    */
    Word16 dn[],         /* i : correlation between target and h[] */
    Word16 rr[][L_CODE], /* i : matrix of autocorrelation          */
    Word16 codvec[]      /* o : algebraic codebook vector          */
);
#endif

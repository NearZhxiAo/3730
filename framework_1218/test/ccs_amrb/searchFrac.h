/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef searchFrac_h
#define searchFrac_h "$Id $"

#include "typedef.h"
/*************************************************************************
 *
 *  FUNCTION:   searchFrac()
 *
 *  PURPOSE: Find fractional pitch
 *
 *  DESCRIPTION:
 *     The function interpolates the normalized correlation at the
 *     fractional positions around lag T0. The position at which the
 *     interpolation function reaches its maximum is the fractional pitch.
 *     Starting point of the search is frac, end point is last_frac.
 *     frac is overwritten with the fractional pitch.
 *
 *************************************************************************/
void searchFrac (
    Word16 *lag,       /* i/o : integer pitch           */
    Word16 *frac,      /* i/o : start point of search -
                                fractional pitch        */
    Word16 last_frac,  /* i   : endpoint of search      */
    Word16 corr[],     /* i   : normalized correlation  */
    Word16 flag3       /* i   : subsample resolution
                                (3: =1 / 6: =0)         */
);
#endif

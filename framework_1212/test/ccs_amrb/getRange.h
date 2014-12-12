/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef getRange_h
#define getRange_h "$Id $"

#include "typedef.h"
/*************************************************************************
 *
 *  FUNCTION:   getRange()
 *
 *  PURPOSE: Sets range around open-loop pitch or integer pitch of last subframe
 *
 *  DESCRIPTION:
 *     Takes integer pitch T0 and calculates a range around it with
 *       t0_min = T0-delta_low  and t0_max = (T0-delta_low) + delta_range
 *     t0_min and t0_max are bounded by pitmin and pitmax
 *
 *************************************************************************/
void getRange (
    Word16 T0,           /* i : integer pitch          */
    Word16 delta_low,    /* i : search start offset    */
    Word16 delta_range,  /* i : search range           */
    Word16 pitmin,       /* i : minimum pitch          */
    Word16 pitmax,       /* i : maximum pitch          */
    Word16 *t0_min,      /* o : search range minimum   */
    Word16 *t0_max       /* o : search range maximum   */
	);
#endif

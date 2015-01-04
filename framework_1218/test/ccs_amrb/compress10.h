/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
********************************************************************************
*/
#ifndef compress10_h
#define compress10_h "$Id $"

#include "typedef.h"

/*************************************************************************
 *
 *  FUNCTION:  compress10()
 *
 *  PURPOSE: compression of three indeces [0..9] to one 10 bit index   
 *           minimizing the phase shift of a bit error.
 *
 *************************************************************************/


Word16 compress10 (
       Word16 pos_indxA, /* i : signs of 4 pulses (signs only)             */
       Word16 pos_indxB, /* i : position index of 8 pulses (pos only)     */
       Word16 pos_indxC  /* i : position and sign of 8 pulses (compressed) */
	   );
#endif

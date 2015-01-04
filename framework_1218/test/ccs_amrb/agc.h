/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.5.0   March 2, 2001
*                                R99   Version 3.2.0                
*                                REL-4 Version 4.0.0                
*
*****************************************************************************
*/
#ifndef agc_h
#define agc_h "$Id $"
 
#include "typedef.h"
 
/*
*****************************************************************************
*                         DEFINITION OF DATA TYPES
*****************************************************************************
*/
typedef struct {
    Word16 past_gain;
} agcState;

/*
**************************************************************************
*
*  Function    : agc
*  Purpose     : Scales the postfilter output on a subframe basis
*  Description : sig_out[n] = sig_out[n] * gain[n];
*                where gain[n] is the gain at the nth sample given by
*                gain[n] = agc_fac * gain[n-1] + (1 - agc_fac) g_in/g_out
*                g_in/g_out is the square root of the ratio of energy at 
*                the input and output of the postfilter.
*
**************************************************************************
*/
int agc (
    agcState *st,      /* i/o : agc state                         */
    Word16 *sig_in,    /* i   : postfilter input signal, (l_trm)  */
    Word16 *sig_out,   /* i/o : postfilter output signal, (l_trm) */
    Word16 agc_fac,    /* i   : AGC factor                        */
    Word16 l_trm       /* i   : subframe size                     */
);

#endif
